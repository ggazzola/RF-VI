/*******************************************************************
   Copyright (C) 2001-7 Leo Breiman, Adele Cutler and Merck & Co., Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*******************************************************************/

/******************************************************************
 * buildtree and findbestsplit routines translated from Leo's
 * original Fortran code.
 *
 *      copyright 1999 by leo Breiman
 *      this is free software and can be used for any purpose.
 *      It comes with no guarantee.
 *
 ******************************************************************/

/*mdim =p*/
 
 
#include <Rmath.h>
#include <R.h>
#include "rf.h"

void regTree(double *x, double *y, int mdim, int nsample, int *lDaughter,
             int *rDaughter,
             double *upper, double *avnode, int *nodestatus, int nrnodes,
             int *treeSize, int nthsize, int mtry, int *mbest, int *cat,
	     double *tgini, int *varUsed, double *ssnode, double *improve_node, double *prob) { /* GGGmod*/
    int i, j, k, m, ncur, *jdex, *nodestart, *nodepop;
    int ndstart, ndend, ndendl, nodecnt, jstat, msplit;
    double d, ss, av, decsplit, ubest, sumnode;

    nodestart = (int *) Calloc(nrnodes, int);
    nodepop   = (int *) Calloc(nrnodes, int);

    /* initialize some arrays for the tree */
    zeroInt(nodestatus, nrnodes);
    zeroInt(nodestart, nrnodes);
    zeroInt(nodepop, nrnodes);
    zeroDouble(avnode, nrnodes);
    zeroDouble(ssnode, nrnodes);
    zeroDouble(improve_node, nrnodes);

    jdex = (int *) Calloc(nsample, int);
    for (i = 1; i <= nsample; ++i) jdex[i-1] = i;

    ncur = 0;
    nodestart[0] = 0;
    nodepop[0] = nsample;
    nodestatus[0] = NODE_TOSPLIT;

    /* compute mean and sum of squares for Y */
    av = 0.0;
    ss = 0.0;
    for (i = 0; i < nsample; ++i) {
	d = y[jdex[i] - 1];
	ss += i * (av - d) * (av - d) / (i + 1);
	av = (i * av + d) / (i + 1);
    }
    avnode[0] = av;
    ssnode[0] = ss;

    /* start main loop */
    for (k = 0; k < nrnodes - 2; ++k) {
		if (k > ncur || ncur >= nrnodes - 2) break;
		/* skip if the node is not to be split */
		if (nodestatus[k] != NODE_TOSPLIT) continue;

		/* initialize for next call to findbestsplit */
		ndstart = nodestart[k];
		ndend = ndstart + nodepop[k] - 1;
		nodecnt = nodepop[k];
		sumnode = nodecnt * avnode[k];
		jstat = 0;
		decsplit = 0.0;

		findBestSplit(x, jdex, y, mdim, nsample, ndstart, ndend, &msplit,
						  &decsplit, &ubest, &ndendl, &jstat, mtry, sumnode,
						  nodecnt, cat, prob); /* GGGmod*/				 
		if (jstat == 1) {
			/* Node is terminal: Mark it as such and move on to the next. */
			nodestatus[k] = NODE_TERMINAL;
			continue;
		}
			/* Found the best split. */
			mbest[k] = msplit;
			varUsed[msplit - 1] = 1;
		upper[k] = ubest;
		tgini[msplit - 1] += decsplit;
		improve_node[k] = decsplit;
		nodestatus[k] = NODE_INTERIOR;

		/* leftnode no.= ncur+1, rightnode no. = ncur+2. */
		nodepop[ncur + 1] = ndendl - ndstart + 1;
		nodepop[ncur + 2] = ndend - ndendl;
		nodestart[ncur + 1] = ndstart;
		nodestart[ncur + 2] = ndendl + 1;

		/* compute mean and sum of squares for the left daughter node */
		av = 0.0;
		ss = 0.0;
		for (j = ndstart; j <= ndendl; ++j) {
			d = y[jdex[j]-1];
			m = j - ndstart;
			ss += m * (av - d) * (av - d) / (m + 1);
			av = (m * av + d) / (m+1);
		}
		avnode[ncur+1] = av;
		ssnode[ncur+1] = ss;
		nodestatus[ncur+1] = NODE_TOSPLIT;
		if (nodepop[ncur + 1] <= nthsize) {
			nodestatus[ncur + 1] = NODE_TERMINAL;
		}

		/* compute mean and sum of squares for the right daughter node */
		av = 0.0;
		ss = 0.0;
		for (j = ndendl + 1; j <= ndend; ++j) {
			d = y[jdex[j]-1];
			m = j - (ndendl + 1);
			ss += m * (av - d) * (av - d) / (m + 1);
			av = (m * av + d) / (m + 1);
		}
		avnode[ncur + 2] = av;
		ssnode[ncur + 2] = ss;
		nodestatus[ncur + 2] = NODE_TOSPLIT;
		if (nodepop[ncur + 2] <= nthsize) {
			nodestatus[ncur + 2] = NODE_TERMINAL;
		}

		/* map the daughter nodes */
		lDaughter[k] = ncur + 1 + 1;
		rDaughter[k] = ncur + 2 + 1;
		/* Augment the tree by two nodes. */
		ncur += 2;
	}
	
    *treeSize = nrnodes;
    for (k = nrnodes - 1; k >= 0; --k) {
        if (nodestatus[k] == 0) (*treeSize)--;
        if (nodestatus[k] == NODE_TOSPLIT) {
            nodestatus[k] = NODE_TERMINAL;
        }
    }
    Free(nodestart);
    Free(jdex);
    Free(nodepop);
}

/*--------------------------------------------------------------*/
void findBestSplit(double *x, int *jdex, double *y, int mdim, int nsample,
		   int ndstart, int ndend, int *msplit, double *decsplit,
		   double *ubest, int *ndendl, int *jstat, int mtry,
		   double sumnode, int nodecnt, int *cat, double *prob) { /* GGGmod*/
    int last, ncat[32], icat[32], lc, nl, nr, npopl, npopr;
    int i, j, kv, l, *mind, *ncase, cnt, ii, iii; /* GGGmod*/
	double *xt, *ut, *v, *yl, sumcat[32], avcat[32], tavcat[32], ubestt, sumTmp, *probTmp, probl, probr;/* GGGmod*/
    double crit, critmax, critvar, suml, sumr, d, critParent;

	/*probl = (double *) Calloc(1, double);/* GGGadd*/
	/*probr = (double *) Calloc(1, double);/* GGGadd*/ 
	probTmp = (double *) Calloc(mdim, double); /* GGGadd*/
    ut = (double *) Calloc(nsample, double);
    xt = (double *) Calloc(nsample, double);
    v  = (double *) Calloc(nsample, double);
    yl = (double *) Calloc(nsample, double);
    mind  = (int *) Calloc(mdim, int); /*GGGCom allocate empty integer vector of p dimensions*/
    ncase = (int *) Calloc(nsample, int);
    zeroDouble(avcat, 32);
    zeroDouble(tavcat, 32);

    /* START BIG LOOP */
    *msplit = -1;
    *decsplit = 0.0;
    critmax = 0.0;
    ubestt = 0.0;
    for (i=0; i < mdim; ++i) { /* GGGadd*/
		mind[i] = i; /* GGGCom mind contains indices 0 through p-1 */
		probTmp[i] = prob[i]; /* GGGadd*/
	}/* GGGadd*/
    last = mdim - 1;

	/* GGG the first position in a vector is 0 */
	cnt = 1;
    while (cnt<=mtry) {
		/*printf("%d", cnt);*/
		/* from here to "End" it's GGGAdd/Mod*/
		j = (int) (unif_rand() * (last+1)); /* GGGCom int here truncates so (int) 0.5 = 0;Choose one index between 0 and "last+1" (probability of last+1 is zero, so actually from 0 to last). 
											 last+1 starts from p and decreases at every iteration*/
		if (unif_rand()>probTmp[j]){
			continue;
		}
		critvar = 0.0;
		sumTmp = 0.0; 
		kv = mind[j];		/*kv is the currently chosen variable */				
		swapInt(mind[j], mind[last]); 
		probl = probTmp[j];
		probr = probTmp[last];
		probTmp[j] = probr;
		probTmp[last] = probl;
		for (ii = 0; i < (last-1); ++ii) { 
			sumTmp = sumTmp + probTmp[ii];
		}
		for (iii = 0; i < (last-1); ++iii) {
			probTmp[iii] = probTmp[iii] * 1/sumTmp;
		}
		cnt++;
		last--;	
		
		/*  End */
		
		lc = cat[kv]; /* GGGcom the kv-th position of vector cat apparently is 1 if the kv-th variable is numerical */
		if (lc == 1) {
			/* numeric variable */
			for (j = ndstart; j <= ndend; ++j) {
			 xt[j] = x[kv + (jdex[j] - 1) * mdim];
			 yl[j] = y[jdex[j] - 1];
			}
		} else {
			/* categorical variable */
				zeroInt(ncat, 32);
			zeroDouble(sumcat, 32);
			for (j = ndstart; j <= ndend; ++j) {
			l = (int) x[kv + (jdex[j] - 1) * mdim];
			sumcat[l - 1] += y[jdex[j] - 1];
			ncat[l - 1] ++;
			}
				/* Compute means of Y by category. */
			for (j = 0; j < lc; ++j) {
			avcat[j] = ncat[j] ? sumcat[j] / ncat[j] : 0.0;
			}
				/* Make the category mean the `pseudo' X data. */
			for (j = 0; j < nsample; ++j) {
			xt[j] = avcat[(int) x[kv + (jdex[j] - 1) * mdim] - 1];
			yl[j] = y[jdex[j] - 1];
			}
		}
			/* copy the x data in this node. */
		for (j = ndstart; j <= ndend; ++j) v[j] = xt[j];
		
		for (j = 1; j <= nsample; ++j) ncase[j - 1] = j;
		
		R_qsort_I(v, ncase, ndstart + 1, ndend + 1);
		
		if (v[ndstart] >= v[ndend]) continue;
		/* ncase(n)=case number of v nth from bottom */
		/* Start from the right and search to the left. */
		critParent = sumnode * sumnode / nodecnt;
		suml = 0.0;
		sumr = sumnode;
		npopl = 0;
		npopr = nodecnt;
		crit = 0.0;
		/* Search through the "gaps" in the x-variable. */
		for (j = ndstart; j <= ndend - 1; ++j) {
			d = yl[ncase[j] - 1];
			suml += d;
			sumr -= d;
			npopl++;
			npopr--;
			if (v[j] < v[j+1]) {
			crit = (suml * suml / npopl) + (sumr * sumr / npopr) -
			  critParent;
			if (crit > critvar) {
				ubestt = (v[j] + v[j+1]) / 2.0;
				critvar = crit;
			}
			}
		}
		if (critvar > critmax) {
			*ubest = ubestt;
			*msplit = kv + 1;
			critmax = critvar;
			for (j = ndstart; j <= ndend; ++j) {
			ut[j] = xt[j];
			}
			if (cat[kv] > 1) {
			for (j = 0; j < cat[kv]; ++j) tavcat[j] = avcat[j];
			}
		}
    }
	
    *decsplit = critmax;

    /* If best split can not be found, set to terminal node and return. */
    if (*msplit != -1) {
        nl = ndstart;
        for (j = ndstart; j <= ndend; ++j) {
            if (ut[j] <= *ubest) {
                nl++;
                ncase[nl-1] = jdex[j];
            }
        }
        *ndendl = imax2(nl - 1, ndstart);
        nr = *ndendl + 1;
        for (j = ndstart; j <= ndend; ++j) {
            if (ut[j] > *ubest) {
                if (nr >= nsample) break;
                nr++;
                ncase[nr - 1] = jdex[j];
            }
	    }
        if (*ndendl >= ndend) *ndendl = ndend - 1;
        for (j = ndstart; j <= ndend; ++j) jdex[j] = ncase[j];

        lc = cat[*msplit - 1];
        if (lc > 1) {
            for (j = 0; j < lc; ++j) {
                icat[j] = (tavcat[j] < *ubest) ? 1 : 0;
            }
            *ubest = pack(lc, icat);
        }
    } else *jstat = 1;

    Free(ncase);
    Free(mind);
    Free(v);
    Free(yl);
    Free(xt);
    Free(ut);
}

/*====================================================================*/
void predictRegTree(double *x, int nsample, int mdim,
		    int *lDaughter, int *rDaughter, int *nodestatus,
                    double *ypred, double *split, double *nodepred,
                    int *splitVar, int treeSize, int *cat, int maxcat,
                    int *nodex) {
    int i, j, k, m, *cbestsplit;
	unsigned int npack;

    /* decode the categorical splits */
    if (maxcat > 1) {
        cbestsplit = (int *) Calloc(maxcat * treeSize, int);
        zeroInt(cbestsplit, maxcat * treeSize);
        for (i = 0; i < treeSize; ++i) {
            if (nodestatus[i] != NODE_TERMINAL && cat[splitVar[i] - 1] > 1) {
                npack = (unsigned int) split[i];
                /* unpack `npack' into bits */
                for (j = 0; npack; npack >>= 1, ++j) {
                    cbestsplit[j + i*maxcat] = npack & 1;
                }
            }
        }
    }

    for (i = 0; i < nsample; ++i) {
	k = 0;
	while (nodestatus[k] != NODE_TERMINAL) { /* go down the tree */
	    m = splitVar[k] - 1;
	    if (cat[m] == 1) {
		k = (x[m + i*mdim] <= split[k]) ?
		    lDaughter[k] - 1 : rDaughter[k] - 1;
	    } else {
	        /* Split by a categorical predictor */
	        k = cbestsplit[(int) x[m + i * mdim] - 1 + k * maxcat] ?
                    lDaughter[k] - 1 : rDaughter[k] - 1;
	    }
	}
	/* terminal node: assign prediction and move on to next */
	ypred[i] = nodepred[k];
	nodex[i] = k + 1;
    }
    if (maxcat > 1) Free(cbestsplit);
}


/*====================================================================*/
int groupRegTreeCross(double *x, int nsample, int mdim, int *nodestatus,
                    double *split,
                    int *splitVar, int treeSize, int *cat, int maxcat,
                    int permVar, int *group, int maxLevel, int *corr) {
    int i, j, k, m, *cbestsplit;
    unsigned int npack;
    int level, dir, power2, ngroup;

    /* decode the categorical splits */
    if (maxcat > 1) {
        cbestsplit = (int *) Calloc(maxcat * treeSize, int);
        zeroInt(cbestsplit, maxcat * treeSize);
        for (i = 0; i < treeSize; ++i) {
            if (nodestatus[i] != NODE_TERMINAL && cat[splitVar[i] - 1] > 1) {
                npack = (unsigned int) split[i];
                /* unpack `npack' into bits */
                for (j = 0; npack; npack >>= 1, ++j) {
                    cbestsplit[j + i*maxcat] = npack & 1;
                }
            }
        }
    }

    ngroup = 1;
    for (i = 0; i < nsample; ++i) {
      level = 0;
      power2 = 1;
      group[i] = 0;
      for (k = 0; k < treeSize; ++k) {
        if (nodestatus[k] == NODE_TERMINAL) /*GGGCom continue in C stands for "go to the next iteration, without completing the current one"*/
          continue; /*GGGCom So this means do not do anything if terminal node*/
        m = splitVar[k] - 1;
        if (m == permVar) /*GGGCom and do not consider partions along variable of interest*/
          continue;
        if (corr[m + permVar * mdim]) { /* split on a correlated variable */
          if (cat[m] == 1) { /*GGGCom if numerical variable*/
            dir = (x[m + i*mdim] <= split[k]);
          } else {
            dir = cbestsplit[(int) x[m + i * mdim] - 1 + k * maxcat];
          }
          if (level < maxLevel) {
            group[i] += power2 * (!dir);  /*GGGCom see Smith paper on extendedForest*/ 
            level++; /*GGGCom looks like counting by number of variables to split on, not number of levels in the tree!!!!*/
            power2 *= 2; 
          } else break;
        }
        if (group[i] + 1 > ngroup)
           ngroup = group[i] + 1;
      }
    }
    if (maxcat > 1) Free(cbestsplit);
    return(ngroup);
}


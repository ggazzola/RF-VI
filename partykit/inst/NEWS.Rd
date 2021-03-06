
\name{NEWS}
\title{NEWS file for the partykit package}

\section{Changes in Version 1.0-4 (2015-09-29)}{
  \itemize{
    \item{Bug fix in \code{plot()} method for \code{constparty} objects.
          In the previous partykit version clipping was accidentally 
	  also applied to the axes labels.}
    \item{For \code{constparty} objects \code{plot(..., type = "simple")}
          did not work correctly whereas \code{plot(as.simpleparty(...))}
	  yielded the desired visualization. Now internally \code{as.simpleparty()}
	  is called also in the former case.}
    \item{The \code{as.simpleparty()} method now preserves p-values from
          \code{constparty} objects (if any).}
    \item{Added a \code{getCall()} method for \code{"party"} objects.}
    \item{In the \code{predict()} method for \code{"lmtree"} and \code{"glmtree"}
          objects the \code{offset} (if any) was sometimes ignored. It is
	  now always used in the prediction.}
  }
}

\section{Changes in Version 1.0-3 (2015-08-14)}{
  \itemize{
    \item{Import \code{logrank_trafo} from \pkg{coin}.}
  }
}

\section{Changes in Version 1.0-2 (2015-07-28)}{
  \itemize{
    \item{\code{nodeprune(..., ids = 1)} did not prune the tree to the root
      node. Fixed now.}
    \item{\code{predict.cforest} used \code{na.omit} instead of \code{na.pass}.}
    \item{\code{predict.party} now features new \code{perm} argument for
          permuting splits in specific variables (useful for computing
          permutation variable importances).}
   \item{\code{NAMESPACE} updates.}
  }
}

\section{Changes in Version 1.0-1 (2015-04-07)}{
  \itemize{
    \item{The support for (generalized) linear model trees with just
      a constant regressor has been improved. Now \code{lmtree(y ~ x1 + x2)}
      is short for \code{lmtree(y ~ 1 | x1 + x2)}, analogously for \code{glmtree()}.
      Plotting now also works properly in this case.}

    \item{The \code{as.party()} method for \code{"rpart"} objects did not
      work if one of the partitioning variables was a \code{"character"}
      variable rather than a \code{"factor"}. A suitable work-around has
      been added.}
      
    \item{The \code{node_barplot()} panel function can now also be used
      for multivariate responses, e.g., when all responses are numeric and
      on the same scale.}

    \item{The package now also includes a new data set \code{HuntingSpiders}
      which is essentially a copy of the \code{spider} data from the package
      \pkg{mvpart} that is currently archived on CRAN. The documentation has
      been improved somewhat and is likely to change further to explain how
      the data has been transformed in De'ath (2002).}

    \item{The survival tree example for the GBSG2 data was broken due to
      the response being (incorrectly) also part of the explanatory variables.
      Fixed by using the latest \pkg{Formula} package (at least version 1.2-1).}
  }
}

\section{Changes in Version 1.0-0 (2015-02-20)}{
  \itemize{
    \item{Version 1.0-0 published. This version is described in the MLOSS paper 
          accepted for publication by the Journal of Machine Learning Research today.}
    \item{The unbiased version of \code{cforest} (with \code{replace = FALSE}) is 
          now the default (as in \pkg{party}).}
    \item{Register all S3 methods in \code{NAMESPACE}.}
  }
}

\section{Changes in Version 0.8-4 (2015-01-06)}{
  \itemize{
    \item{Extended \code{mob()} interface by a \code{cluster} argument. This can be
    a vector (numeric, integer, factor) with cluster IDs that are
    then passed on to the 'fit' function (if supported) and used
    for clustering the covariance matrix in the parameter stability
    tests. \code{lmtree()} and \code{glmtree()} hence both gained a \code{cluster}
    argument which is used only for cluster covariances but not
    for the model estimation (i.e., corresponding to a working
    independence model).}

    \item{Optionally, the parameters' variance-covariance matrix in \code{mob()}
    can now be estimated by the sandwich matrix instead of the default
    outer-product-of-gradients (OPG) matrix or the information matrix.}

    \item{Reimplementation of \code{cforest()} available with extended 
    prediction facilities. Both the internal representation and the user interface
    are still under development are likely to change in future versions.}
    
    \item{Added multicore support to \code{mob()}, \code{ctree()}, and \code{cforest()}.
    If control argument \code{cores} is specified (e.g., \code{cores = 4}) then the
    search for the best variable or split point (often involving numerous model fits in
    \code{mob()} or resampling in \code{ctree()}) is carried out using \code{parallel::mclapply()}
    rathern than sequential \code{for()} or \code{sapply()}. Additionally, other
    \code{applyfun}s can be provided, e.g., using networks of workstations etc.}

    \item{Bug fix in \code{mob()} that occurred when regressor variables and
    partitioning variables overlapped and were not sorted in the
    underlying model frame.}
  }
}

\section{Changes in Version 0.8-3 (2014-12-15)}{
  \itemize{
    \item{\pkg{mvpart} was archived 2014-12-15.}
  }
}

\section{Changes in Version 0.8-2 (2014-09-12)}{
  \itemize{
    \item{Fixed an uninitialized memory issue reported by valgrind.}
  }
}

\section{Changes in Version 0.8-1 (2014-09-08)}{
  \itemize{

  \item{partykit now depends on R version >= 3.1.0 in order to import the
        \code{depth()} generic from the \pkg{grid} package.}

  \item{The print methods for \code{party}/\code{partynode} objects with only a root node
    was modified. Now, the terminal panel function is also applied
    if there is only a root node (while previously it was not).}

  \item{ \code{ctree()} now catches \code{sum(weights) <= 1} situations before they 
    lead to an error.}

  \item{ Code from suggested packages is included by using \code{::} syntax as
    required by recent R versions.}
    
  \item{ Argument \code{ytrafo} of \code{ctree()} can now be a function which will be
    updated in every node.}
    
  \item{ A small demo briefly illustrating some memory and speed properties
    has been added. It can be run interactively via
    \code{demo("memory-speed", package = "partykit").}}
    
  \item{ Section 3 of the "constparty" vignette now shows how properties of
    a new tree algorithm can be assessed by using \pkg{partykit} building
    blocks.}
  }
}


\section{Changes in Version 0.8-0 (2014-03-27)}{
  \itemize{

  \item{Major improved version of \pkg{partykit}. The previously existing functions
    in the package were tested and enhanced, new functions and
    extensive vignettes added.}

  \item{Extended and improved introductory documentation. The basic classes
    and class constructors \code{partynode}/\code{partysplit}/\code{party} are introduced in 
    much more detail now in \code{vignette("partykit", package = "partykit")}.}
    
  \item{The class \code{constparty} (inheriting from \code{party}) for representing \code{party}
    objects with constant fits in the nodes (along with coercion methods
    for \code{rpart}, \code{J48}, etc.) is now described in more detail in the new
    \code{vignette("constparty", package = "partykit")}.}

  \item{The package now includes a reimplementation of the model-based
    recursive partitioning algorithm (MOB) using \pkg{partykit} infrastructure.
    The generic algorithm, the corresponding convenience interfaces
    \code{lmtree()} and \code{glmtree()} as well as various illustrations and possible
    extensions are described in detail in the new
    \code{vignette("mob", package = "partykit")}.}

  \item{Improved implementation of conditional inference trees (CTree), see
    the new \code{vignette("ctree", package = "partykit")} for details.}

  \item{New \code{nodeprune()} generic for pruning nodes in all \code{party} trees and
    \code{partynode} objects.}

  \item{Deal with empty levels in \code{ctree()} for \code{teststat = "quad"}
    (bug reported by Wei-Yin Loh <loh_at_stat.wisc.edu>).}

  \item{In \code{predict()} method for \code{constparty} objects, \code{type = "prob"} now returns
    ECDF for numeric responses and \code{type = "response"} the (weighted) mean.}

  \item{New panel function \code{node_ecdf()} for plotting empirical cumulative
    distribution functions in the terminal leaves of \code{constparty} trees.}
  }
}

\section{Changes in Version 0.1-6 (2013-09-03)}{
  \itemize{

  \item{Bug fix in \code{as.party()} method for J48 trees with ordered factors.}
 }
}

\section{Changes in Version 0.1-5 (2013-03-22)}{
  \itemize{

  \item{Fix C code problems reported by clang under OS X.}
  }
}

\section{Changes in Version 0.1-4 (2012-06-05)}{
  \itemize{

  \item{Added \code{node_surv()} for plotting survival ctrees. Accompanying
    infrastructure for survival trees was enhanced.}
    
  \item{\code{ctree()} now checks for (and does not allow) \code{x >= max(x)} splits.}
  }
}

\section{Changes in Version 0.1-3 (2012-01-11)}{
  \itemize{

  \item{Added \pkg{ipred} to the list of suggested packages due to usage of
        GlaucomaM and GBSG2 data in tests/examples.}
  }
}


\section{Changes in Version 0.1-2 (2011-12-18)}{
  \itemize{

  \item{The \code{node_terminal()} panel-generating function is now customizable
    by a FUN argument that is passed to \code{formatinfo()}.}

  \item{The \code{plot()} method for \code{simpleparty} object now sets up a formatting
    function passed to \code{formatinfo()}, both in \code{print()} and \code{plot()}.}

  \item{Fixed bug in \code{pmmlTreeModel()} for processing label IDS in splits when
    not all levels are present.}
  
  \item{Cleaned up unused variables in C code and partial argument matching
    in R code.}
  }
}  

\section{Changes in Version 0.1-1 (2011-09-29)}{
  \itemize{

  \item{First CRAN release.}
  
  \item{See \code{vignette("partykit", package = "partykit")} for a (somewhat rough)
    introduction to the package and its classes/methods.}
  }
}

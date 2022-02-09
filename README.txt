THE RBO INDEFINITE RANK SIMILARITY METRIC
=========================================

* QUICK INSTALLATION INSTRUCTIONS

Type "make" in the current directory.  This will produce an executable
called "rbo".  Type "make test" to run the tests, if you wish.  Run
"rbo -h" to get a brief help.  Copy the executable to the desired
location.

* DESCRIPTION

This package implements the Rank-Biased Overlap (RBO) indefinite rank
similarity measure, as described in:

    @article{wmz10:acmtois,
        author = "Webber, William and Moffat, Alistair and Zobel, Justin",
        title = "A similarity measure for indefinite rankings",
        journal = "ACM Transactions on Information Systems",
        volume = 28,
        number = 4,
        year = {2010},
        month = nov,
        pages = {20:1--38}
    }

Please cite the above paper if you use the RBO metric.

The package also implements a few other rank similarity metrics;
currently, Kendall's tau and Average Overlap (see above paper for the
latter).

The implementation of RBO calculates extrapolated RBO.  Rankings of
different lengths are handle.  Ties in the ranking are not currently
supported, due to uncertainty over what the import format should be.
The error residuals are not given.

The implementation of Kendall's tau uses the merge sort method
described by:

    @article{knight66:jasa,
        title = "A Computer Method for Calculating Kendall's Tau with Ungrouped Data",
        author = "Knight, William R.",
        journal = "Journal of the American Statistical Association",
        volume = 61,
        number = 314,
        pages = {436--439},
        year = {1966}
    }

Knight's algorithm is O(n log n), compared to O(n ^ 2) for more naive
implementations (such as that used in the R implementation of
Kendall's tau).

More metrics and features are planned in future versions.

* USAGE

   rbo [-m metric] [-p <p-value>] <rnk1> <rnk2>

Metrics supported are "rbo" (the default), "ao" (average overlap), and
"tau" (Kendall's tau).  The "p" parameter applies to RBO only; the
default value is 0.98.  The arguments <rnk1> and <rnk2> are files
containing object indentifiers, one per line.   The rankings of the
objects are taken from their position in the file.  Specifying object
ranking implicitly by giving an object a score (as, for instance, in
the R function 'cor(a, b, method="kendall"') is not currently
supported.

* MAINTAINER

This code is written and maintained by:

   William Webber <william@williamwebber.com>

Please send bugs and comments to the above address.

See COPYRIGHT.txt for copyright and licensing conditions
(summary: this is open source, under an MIT license).

<!-- README.md is generated from README.Rmd. Please edit that file -->
crafter is eventually going to be a R pacakge to work with PCAP dumps

Life's too short to export to CSV/XML. There's no reason R should not be able to read binary PCAP data.

[What is a PCAP?](https://en.wikipedia.org/wiki/Pcap)

You need the [crafter C++ library](https://github.com/pellegre/libcrafter), which lists the other dependencies.

If there's any hope for this to run on Windows (`libcrafter` supports Windows) it will be due to a Windows + (prbly some infosec) + `#rstats` person tagging along on this project.

The following functions are implemented:

-   `read_pcap`: for now, it pulls IP packet metadata from PCAP dumps. The whole works is planned, tho.

### News

-   Version 0.0.0.9000 released

### Installation

``` r
devtools::install_github("hrbrmstr/crafter")
```

### Usage

``` r
library(crafter)

# current verison
packageVersion("crafter")
#> [1] '0.0.0.9000'
```

### Test Results

``` r
library(crafter)
library(testthat)

date()
#> [1] "Thu Aug 13 16:34:52 2015"

test_dir("tests/")
#> testthat results ========================================================================================================
#> OK: 0 SKIPPED: 0 FAILED: 0
#> 
#> DONE
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.

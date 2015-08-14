<!-- README.md is generated from README.Rmd. Please edit that file -->
    __________________________oooo__oo____________________
    _ooooo__oo_ooo___ooooo___oo_____oo_____ooooo__oo_ooo__
    oo___oo_ooo___o_oo___oo_ooooo__oooo___oo____o_ooo___o_
    oo______oo______oo___oo_oo______oo____ooooooo_oo______
    oo______oo______oo___oo_oo______oo__o_oo______oo______
    _ooooo__oo_______oooo_o_oo_______ooo___ooooo__oo______
    ______________________________________________________

crafter : Tools to Analyze and Visualize Network Packet Capture (PCAP) Files

Life's too short to export to CSV/XML. There's no reason R should not be able to read binary PCAP data.

[What is a PCAP?](https://en.wikipedia.org/wiki/Pcap)

You need the [crafter C++ library](https://github.com/pellegre/libcrafter) installed and their site lists the other dependencies.

If there's any hope for this to run on Windows (`libcrafter` supports Windows) it will be due to a Windows + (prbly some infosec) + `#rstats` person tagging along on this project.

The following functions are implemented:

-   `read_pcap`: Read in a packet capture file
-   `summary.crafter`: Print summary info about a packet capture

### News

-   Version 0.1.0.9000 : very basic functionality!
-   Version 0.0.1.9000 : refactor into classed environment
-   Version 0.0.0.9000 : released

### Installation

``` r
devtools::install_github("hrbrmstr/crafter")
```

### Usage

``` r
library(crafter)

# current verison
packageVersion("crafter")
#> [1] '0.1.0.9000'

library(crafter)
library(dplyr)
#> 
#> Attaching package: 'dplyr'
#> 
#> The following objects are masked from 'package:stats':
#> 
#>     filter, lag
#> 
#> The following objects are masked from 'package:base':
#> 
#>     intersect, setdiff, setequal, union
library(ggplot2)

# read in the "honeybot" packet capture from the "Capture the hacker 2013"
# competition (by Dr. David Day of Sheffield Hallam University) http://www.snaketrap.co.uk/
hbot <- read_pcap(system.file("pcaps/hbot.pcap", package="crafter"))

# high level statistics
summary(hbot)
#> File
#>  Capture file  : /Library/Frameworks/R.framework/Versions/3.2/Resources/library/crafter/pcaps/hbot.pcap
#>  Filter applied: [none]
#>  Length        : 1204229 bytes
#> 
#> Time
#>  First packet: 2013-01-09 15:33:20
#>  Last packet : 2013-01-12 15:19:20
#> 
#> Statistics
#>  Packets      : 5450
#>  Time span (s): 258360.620
#>  Average pps  : 0.0

# look at general packet info
head(hbot$packet_info(), 15)
#>        tv_sec tv_usec layer_count                                                               protocols packet_size
#> 1  1357745600  129081           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted          62
#> 2  1357745600  139696           3                                                   ARP,Ethernet,RawLayer          60
#> 3  1357745600  139723           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted          62
#> 4  1357745600  278594           3                                                         Ethernet,IP,TCP          54
#> 5  1357745600  279116           3                                                         Ethernet,IP,TCP          54
#> 6  1357745600  279178           3                                                         Ethernet,IP,TCP          54
#> 7  1357745600  396873           4                                                Ethernet,IP,RawLayer,TCP          95
#> 8  1357745600  397813           3                                                         Ethernet,IP,TCP          54
#> 9  1357745600  547062           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted          62
#> 10 1357745600  547139           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted          62
#> 11 1357745600  549150           3                                                         Ethernet,IP,TCP          54
#> 12 1357745600  695985           3                                                         Ethernet,IP,TCP          54
#> 13 1357745600  696408           3                                                         Ethernet,IP,TCP          54
#> 14 1357745600  696526           3                                                         Ethernet,IP,TCP          54
#> 15 1357745600  819813           4                                                Ethernet,IP,RawLayer,TCP          79

# look at the IP layer packets
head(hbot$get_layer("IP"), 20)
#>        tv_sec tv_usec         time           src           dst protocol protocol_name size header_len total_len ttl
#> 1  1357745600  129081 0.000000e+00  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115
#> 2  1357745600  139723 1.064205e-02 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128
#> 3  1357745600  278594 1.388710e-01  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 4  1357745600  279116 5.218983e-04  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 5  1357745600  279178 6.198883e-05 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128
#> 6  1357745600  396873 1.176951e-01 192.168.0.200  70.84.211.98        6           TCP   95          5        81 128
#> 7  1357745600  397813 9.400845e-04 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128
#> 8  1357745600  547062 1.492488e-01  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115
#> 9  1357745600  547139 7.700920e-05 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128
#> 10 1357745600  549150 2.011061e-03  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 11 1357745600  695985 1.468351e-01  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 12 1357745600  696408 4.229546e-04  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 13 1357745600  696526 1.180172e-04 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128
#> 14 1357745600  819813 1.232870e-01 192.168.0.200  70.84.211.98        6           TCP   79          5        65 128
#> 15 1357745600  822347 2.533913e-03 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128
#> 16 1357745600  957518 1.351712e-01  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 17 1357745600  958101 5.829334e-04  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115
#> 18 1357745600  958347 2.460480e-04 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128
#> 19 1357745601   90287 1.319399e-01  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115
#> 20 1357745601   91009 7.219315e-04  70.84.211.98 192.168.0.200        6           TCP  190          5       176 115
#>    flags flag_bits dscp frag_ofs
#> 1      2       010    0        0
#> 2      2       010    0        0
#> 3      2       010    0        0
#> 4      2       010    0        0
#> 5      2       010    0        0
#> 6      2       010    0        0
#> 7      2       010    0        0
#> 8      2       010    0        0
#> 9      2       010    0        0
#> 10     2       010    0        0
#> 11     2       010    0        0
#> 12     2       010    0        0
#> 13     2       010    0        0
#> 14     2       010    0        0
#> 15     2       010    0        0
#> 16     2       010    0        0
#> 17     2       010    0        0
#> 18     2       010    0        0
#> 19     2       010    0        0
#> 20     2       010    0        0

# see the protocol distribution
hbot$get_layer("IP") %>% 
  count(protocol_name) %>% 
  ggplot(aes(x=protocol_name, y=n)) + 
  geom_bar(stat="identity") + 
  labs(x=NULL, title="Honeybot IP Protocols") + 
  theme_bw()
```

<img src="README-unnamed-chunk-4-1.png" title="" alt="" width="672" />

### Test Results

``` r
library(crafter)
library(testthat)

date()
#> [1] "Fri Aug 14 16:28:49 2015"

test_dir("tests/")
#> testthat results ========================================================================================================
#> OK: 0 SKIPPED: 0 FAILED: 0
#> 
#> DONE
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.

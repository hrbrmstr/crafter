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

You can find some sample PCAP files:

-   [Netresec](https://wiki.wireshark.org/SampleCaptures)
-   [Wireshark](https://wiki.wireshark.org/SampleCaptures)

The following functions are implemented:

-   `read_pcap`: Read in a packet capture file
-   `summary.crafter`: Print summary info about a packet capture

(There are actually more but they're inside the pcap object and I just need to get them exposed. See the example below for usage.)

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
#>        tv_sec tv_usec       time layer_count                                                               protocols
#> 1  1357745600  129081 0.00000000           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 2  1357745600  139696 0.01061487           3                                                   ARP,Ethernet,RawLayer
#> 3  1357745600  139723 0.01064205           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 4  1357745600  278594 0.14951301           3                                                         Ethernet,IP,TCP
#> 5  1357745600  279116 0.15003490           3                                                         Ethernet,IP,TCP
#> 6  1357745600  279178 0.15009689           3                                                         Ethernet,IP,TCP
#> 7  1357745600  396873 0.26779199           4                                                Ethernet,IP,RawLayer,TCP
#> 8  1357745600  397813 0.26873207           3                                                         Ethernet,IP,TCP
#> 9  1357745600  547062 0.41798091           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 10 1357745600  547139 0.41805792           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 11 1357745600  549150 0.42006898           3                                                         Ethernet,IP,TCP
#> 12 1357745600  695985 0.56690407           3                                                         Ethernet,IP,TCP
#> 13 1357745600  696408 0.56732702           3                                                         Ethernet,IP,TCP
#> 14 1357745600  696526 0.56744504           3                                                         Ethernet,IP,TCP
#> 15 1357745600  819813 0.69073200           4                                                Ethernet,IP,RawLayer,TCP
#>    packet_size
#> 1           62
#> 2           60
#> 3           62
#> 4           54
#> 5           54
#> 6           54
#> 7           95
#> 8           54
#> 9           62
#> 10          62
#> 11          54
#> 12          54
#> 13          54
#> 14          54
#> 15          79

# look at the IP layer packets
head(hbot$get_layer("IP"), 20)
#>        tv_sec tv_usec       time           src           dst protocol protocol_name size header_len total_len ttl flags
#> 1  1357745600  129081 0.00000000  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115     2
#> 2  1357745600  139723 0.01064205 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128     2
#> 3  1357745600  278594 0.14951301  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 4  1357745600  279116 0.15003490  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 5  1357745600  279178 0.15009689 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128     2
#> 6  1357745600  396873 0.26779199 192.168.0.200  70.84.211.98        6           TCP   95          5        81 128     2
#> 7  1357745600  397813 0.26873207 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128     2
#> 8  1357745600  547062 0.41798091  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115     2
#> 9  1357745600  547139 0.41805792 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128     2
#> 10 1357745600  549150 0.42006898  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 11 1357745600  695985 0.56690407  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 12 1357745600  696408 0.56732702  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 13 1357745600  696526 0.56744504 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128     2
#> 14 1357745600  819813 0.69073200 192.168.0.200  70.84.211.98        6           TCP   79          5        65 128     2
#> 15 1357745600  822347 0.69326591 192.168.0.200  70.84.211.98        6           TCP   54          5        40 128     2
#> 16 1357745600  957518 0.82843709  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 17 1357745600  958101 0.82902002  70.84.211.98 192.168.0.200        6           TCP   62          5        48 115     2
#> 18 1357745600  958347 0.82926607 192.168.0.200  70.84.211.98        6           TCP   62          5        48 128     2
#> 19 1357745601   90287 0.96120596  70.84.211.98 192.168.0.200        6           TCP   54          5        40 115     2
#> 20 1357745601   91009 0.96192789  70.84.211.98 192.168.0.200        6           TCP  190          5       176 115     2
#>    flag_bits dscp frag_ofs
#> 1        010    0        0
#> 2        010    0        0
#> 3        010    0        0
#> 4        010    0        0
#> 5        010    0        0
#> 6        010    0        0
#> 7        010    0        0
#> 8        010    0        0
#> 9        010    0        0
#> 10       010    0        0
#> 11       010    0        0
#> 12       010    0        0
#> 13       010    0        0
#> 14       010    0        0
#> 15       010    0        0
#> 16       010    0        0
#> 17       010    0        0
#> 18       010    0        0
#> 19       010    0        0
#> 20       010    0        0

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
#> [1] "Fri Aug 14 17:04:58 2015"

test_dir("tests/")
#> testthat results ========================================================================================================
#> OK: 0 SKIPPED: 0 FAILED: 0
#> 
#> DONE
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.

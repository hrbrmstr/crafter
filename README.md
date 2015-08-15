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
#>    num     tv_sec tv_usec layer_count                                                               protocols
#> 1    1 1357913756  642112           4                                                Ethernet,IP,RawLayer,UDP
#> 2    2 1357913756  652518           4                                                Ethernet,IP,RawLayer,UDP
#> 3    3 1357913756  661374           4                                                Ethernet,IP,RawLayer,UDP
#> 4    4 1357913756  768192           4                                                Ethernet,IP,RawLayer,UDP
#> 5    5 1357913763   22726           4                                                Ethernet,IP,RawLayer,UDP
#> 6    6 1357913763   32152           4                                                Ethernet,IP,RawLayer,UDP
#> 7    7 1357913763   34026           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 8    8 1357913763   60454           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#> 9    9 1357913763   60517           3                                                         Ethernet,IP,TCP
#> 10  10 1357913763   61083           4                                                Ethernet,IP,RawLayer,TCP
#> 11  11 1357913763   89809           3                                                         Ethernet,IP,TCP
#> 12  12 1357913763   90103           4                                                Ethernet,IP,RawLayer,TCP
#> 13  13 1357913763  137534           4                                                Ethernet,IP,RawLayer,UDP
#> 14  14 1357913763  147996           4                                                Ethernet,IP,RawLayer,UDP
#> 15  15 1357913763  149377           7 Ethernet,IP,TCP,TCPOptionMaxSegSize,TCPOptionPad,TCPOptionSACKPermitted
#>    packet_size
#> 1           83
#> 2          195
#> 3           88
#> 4           61
#> 5           79
#> 6           95
#> 7           62
#> 8           62
#> 9           54
#> 10         162
#> 11          54
#> 12         452
#> 13          80
#> 14          96
#> 15          62

# look at the IP layer packets
head(hbot$get_layer("IP"), 20)
#>    num     tv_sec tv_usec            src            dst protocol protocol_name size header_len total_len ttl flags
#> 1    1 1357913756  642112  192.168.0.200  194.168.4.100       17           UDP   83          5        69 128     0
#> 2    2 1357913756  652518  194.168.4.100  192.168.0.200       17           UDP  195          5       181 253     2
#> 3    3 1357913756  661374  192.168.0.200  199.66.201.20       17           UDP   88          5        74 128     0
#> 4    4 1357913756  768192  199.66.201.20  192.168.0.200       17           UDP   61          5        47  51     2
#> 5    5 1357913763   22726  192.168.0.200  194.168.4.100       17           UDP   79          5        65 128     0
#> 6    6 1357913763   32152  194.168.4.100  192.168.0.200       17           UDP   95          5        81 253     2
#> 7    7 1357913763   34026  192.168.0.200 91.199.212.171        6           TCP   62          5        48 128     2
#> 8    8 1357913763   60454 91.199.212.171  192.168.0.200        6           TCP   62          5        48  49     2
#> 9    9 1357913763   60517  192.168.0.200 91.199.212.171        6           TCP   54          5        40 128     2
#> 10  10 1357913763   61083  192.168.0.200 91.199.212.171        6           TCP  162          5       148 128     2
#> 11  11 1357913763   89809 91.199.212.171  192.168.0.200        6           TCP   54          5        40  49     2
#> 12  12 1357913763   90103 91.199.212.171  192.168.0.200        6           TCP  452          5       438  49     2
#> 13  13 1357913763  137534  192.168.0.200  194.168.4.100       17           UDP   80          5        66 128     0
#> 14  14 1357913763  147996  194.168.4.100  192.168.0.200       17           UDP   96          5        82 253     2
#> 15  15 1357913763  149377  192.168.0.200   178.255.82.1        6           TCP   62          5        48 128     2
#> 16  16 1357913763  228146  192.168.0.200 91.199.212.171        6           TCP   54          5        40 128     2
#> 17  17 1357913763  247620   178.255.82.1  192.168.0.200        6           TCP   62          5        48  49     2
#> 18  18 1357913763  247671  192.168.0.200   178.255.82.1        6           TCP   54          5        40 128     2
#> 19  19 1357913763  248170  192.168.0.200   178.255.82.1        6           TCP  187          5       173 128     2
#> 20  20 1357913763  348497   178.255.82.1  192.168.0.200        6           TCP   54          5        40  49     2
#>    flag_bits dscp frag_ofs
#> 1        000    0        0
#> 2        010    0        0
#> 3        000    0        0
#> 4        010    0        0
#> 5        000    0        0
#> 6        010    0        0
#> 7        010    0        0
#> 8        010    0        0
#> 9        010    0        0
#> 10       010    0        0
#> 11       010    0        0
#> 12       010    0        0
#> 13       000    0        0
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
#> [1] "Fri Aug 14 21:27:38 2015"

test_dir("tests/")
#> testthat results ========================================================================================================
#> OK: 0 SKIPPED: 0 FAILED: 0
#> 
#> DONE
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.

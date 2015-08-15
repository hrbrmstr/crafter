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

-   Version 0.1.1.9000 : restored packet number in data frame & added TCP layer
-   Version 0.1.0.9000 : very basic functionality (IP layer)!
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
#>    num     tv_sec tv_usec            src            dst protocol_name size header_len total_len ttl flags flag_bits
#> 1    1 1357913756  642112  192.168.0.200  194.168.4.100           UDP   83          5        69 128     0       000
#> 2    2 1357913756  652518  194.168.4.100  192.168.0.200           UDP  195          5       181 253     2       010
#> 3    3 1357913756  661374  192.168.0.200  199.66.201.20           UDP   88          5        74 128     0       000
#> 4    4 1357913756  768192  199.66.201.20  192.168.0.200           UDP   61          5        47  51     2       010
#> 5    5 1357913763   22726  192.168.0.200  194.168.4.100           UDP   79          5        65 128     0       000
#> 6    6 1357913763   32152  194.168.4.100  192.168.0.200           UDP   95          5        81 253     2       010
#> 7    7 1357913763   34026  192.168.0.200 91.199.212.171           TCP   62          5        48 128     2       010
#> 8    8 1357913763   60454 91.199.212.171  192.168.0.200           TCP   62          5        48  49     2       010
#> 9    9 1357913763   60517  192.168.0.200 91.199.212.171           TCP   54          5        40 128     2       010
#> 10  10 1357913763   61083  192.168.0.200 91.199.212.171           TCP  162          5       148 128     2       010
#> 11  11 1357913763   89809 91.199.212.171  192.168.0.200           TCP   54          5        40  49     2       010
#> 12  12 1357913763   90103 91.199.212.171  192.168.0.200           TCP  452          5       438  49     2       010
#> 13  13 1357913763  137534  192.168.0.200  194.168.4.100           UDP   80          5        66 128     0       000
#> 14  14 1357913763  147996  194.168.4.100  192.168.0.200           UDP   96          5        82 253     2       010
#> 15  15 1357913763  149377  192.168.0.200   178.255.82.1           TCP   62          5        48 128     2       010
#> 16  16 1357913763  228146  192.168.0.200 91.199.212.171           TCP   54          5        40 128     2       010
#> 17  17 1357913763  247620   178.255.82.1  192.168.0.200           TCP   62          5        48  49     2       010
#> 18  18 1357913763  247671  192.168.0.200   178.255.82.1           TCP   54          5        40 128     2       010
#> 19  19 1357913763  248170  192.168.0.200   178.255.82.1           TCP  187          5       173 128     2       010
#> 20  20 1357913763  348497   178.255.82.1  192.168.0.200           TCP   54          5        40  49     2       010
#>    dscp frag_ofs
#> 1     0        0
#> 2     0        0
#> 3     0        0
#> 4     0        0
#> 5     0        0
#> 6     0        0
#> 7     0        0
#> 8     0        0
#> 9     0        0
#> 10    0        0
#> 11    0        0
#> 12    0        0
#> 13    0        0
#> 14    0        0
#> 15    0        0
#> 16    0        0
#> 17    0        0
#> 18    0        0
#> 19    0        0
#> 20    0        0

# look at the TCP layer packets
head(hbot$get_layer("TCP"), 20)
#>    num     tv_sec tv_usec            src            dst protocol_name srcport dstport     seqnum     acknum   fin   syn
#> 1    7 1357913763   34026  192.168.0.200 91.199.212.171           TCP    1033      80 3814599572          0 FALSE  TRUE
#> 2    8 1357913763   60454 91.199.212.171  192.168.0.200           TCP      80    1033 1804036196 3814599573 FALSE  TRUE
#> 3    9 1357913763   60517  192.168.0.200 91.199.212.171           TCP    1033      80 3814599573 1804036197 FALSE FALSE
#> 4   10 1357913763   61083  192.168.0.200 91.199.212.171           TCP    1033      80 3814599573 1804036197 FALSE FALSE
#> 5   11 1357913763   89809 91.199.212.171  192.168.0.200           TCP      80    1033 1804036197 3814599681 FALSE FALSE
#> 6   12 1357913763   90103 91.199.212.171  192.168.0.200           TCP      80    1033 1804036197 3814599681 FALSE FALSE
#> 7   15 1357913763  149377  192.168.0.200   178.255.82.1           TCP    1035      80 1589035848          0 FALSE  TRUE
#> 8   16 1357913763  228146  192.168.0.200 91.199.212.171           TCP    1033      80 3814599681 1804036595 FALSE FALSE
#> 9   17 1357913763  247620   178.255.82.1  192.168.0.200           TCP      80    1035 2486095265 1589035849 FALSE  TRUE
#> 10  18 1357913763  247671  192.168.0.200   178.255.82.1           TCP    1035      80 1589035849 2486095266 FALSE FALSE
#> 11  19 1357913763  248170  192.168.0.200   178.255.82.1           TCP    1035      80 1589035849 2486095266 FALSE FALSE
#> 12  20 1357913763  348497   178.255.82.1  192.168.0.200           TCP      80    1035 2486095266 1589035982 FALSE FALSE
#> 13  21 1357913763  348961   178.255.82.1  192.168.0.200           TCP      80    1035 2486095266 1589035982 FALSE FALSE
#> 14  22 1357913763  349525   178.255.82.1  192.168.0.200           TCP      80    1035 2486096726 1589035982 FALSE FALSE
#> 15  23 1357913763  349762  192.168.0.200   178.255.82.1           TCP    1035      80 1589035982 2486098186 FALSE FALSE
#> 16  24 1357913763  350125   178.255.82.1  192.168.0.200           TCP      80    1035 2486098186 1589035982 FALSE FALSE
#> 17  25 1357913763  448811   178.255.82.1  192.168.0.200           TCP      80    1035 2486099646 1589035982 FALSE FALSE
#> 18  26 1357913763  449199  192.168.0.200   178.255.82.1           TCP    1035      80 1589035982 2486099703 FALSE FALSE
#> 19  27 1357913765   88525 91.199.212.171  192.168.0.200           TCP      80    1033 1804036595 3814599681  TRUE FALSE
#> 20  28 1357913765   88703  192.168.0.200 91.199.212.171           TCP    1033      80 3814599681 1804036596 FALSE FALSE
#>      rst   psh   ack   urg   ece   cwr
#> 1  FALSE FALSE FALSE FALSE FALSE FALSE
#> 2  FALSE FALSE  TRUE FALSE FALSE FALSE
#> 3  FALSE FALSE  TRUE FALSE FALSE FALSE
#> 4  FALSE  TRUE  TRUE FALSE FALSE FALSE
#> 5  FALSE FALSE  TRUE FALSE FALSE FALSE
#> 6  FALSE  TRUE  TRUE FALSE FALSE FALSE
#> 7  FALSE FALSE FALSE FALSE FALSE FALSE
#> 8  FALSE FALSE  TRUE FALSE FALSE FALSE
#> 9  FALSE FALSE  TRUE FALSE FALSE FALSE
#> 10 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 11 FALSE  TRUE  TRUE FALSE FALSE FALSE
#> 12 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 13 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 14 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 15 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 16 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 17 FALSE  TRUE  TRUE FALSE FALSE FALSE
#> 18 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 19 FALSE FALSE  TRUE FALSE FALSE FALSE
#> 20 FALSE FALSE  TRUE FALSE FALSE FALSE

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
#> [1] "Fri Aug 14 22:40:08 2015"

test_dir("tests/")
#> testthat results ========================================================================================================
#> OK: 0 SKIPPED: 0 FAILED: 0
#> 
#> DONE
```

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.

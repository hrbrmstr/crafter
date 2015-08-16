#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

#include "crafter_types.h"

using namespace Rcpp;
using namespace std;
using namespace Crafter;

// get the icmp layer from a pcap as a data frame
//
// @param pcap pcap
// [[Rcpp::export]]
DataFrame get_icmp_layer(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num; num.reserve(pck_cont.size());
  std::vector<int> tssec; tssec.reserve(pck_cont.size());
  std::vector<int> tsusec; tsusec.reserve(pck_cont.size());
  std::vector<std::string> src; src.reserve(pck_cont.size());
  std::vector<std::string> dst; dst.reserve(pck_cont.size());
  std::vector<int> protocol; protocol.reserve(pck_cont.size());
  std::vector<unsigned int> identifier; identifier.reserve(pck_cont.size());
  std::vector<unsigned int> seqnum; seqnum.reserve(pck_cont.size());
  std::vector<int> icmptype; icmptype.reserve(pck_cont.size());
  std::vector<int> code; code.reserve(pck_cont.size());
  std::vector<unsigned int> chksum; chksum.reserve(pck_cont.size());


  std::vector<Packet*>::iterator it_pck;
  timeval ts;

  int i = 0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    ICMP* icmp_layer = (*it_pck)->GetLayer<ICMP>();
    IP* ip_layer = (*it_pck)->GetLayer<IP>();

    if (icmp_layer) {

      ts = (*it_pck)->GetTimestamp();

      num.push_back(i+1);
      tssec.push_back(ts.tv_sec);
      tsusec.push_back(ts.tv_usec);
      src.push_back(ip_layer->GetSourceIP());
      dst.push_back(ip_layer->GetDestinationIP());
      protocol.push_back((int)ip_layer->GetProtocol());
      identifier.push_back((unsigned int)icmp_layer->GetIdentifier());
      seqnum.push_back((unsigned int)icmp_layer->GetSequenceNumber());
      icmptype.push_back((int)icmp_layer->GetType());
      code.push_back((int)icmp_layer->GetCode());
      chksum.push_back((unsigned int)icmp_layer->GetCheckSum());

    }

    i++;

  }

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["src"] = src,
                           _["dst"] = dst,
                           _["protocol"] = protocol,
                           _["identifier"] = identifier,
                           _["seqnum"] = seqnum,
                           _["icmptype"] = icmptype,
                           _["code"] = code,
                           _["chksum"] = chksum,
                           _["stringsAsFactors"] = false);

}

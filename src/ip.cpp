#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

#include "crafter_types.h"

using namespace Rcpp;
using namespace std;
using namespace Crafter;


// get the ip layer from a pcap as a data frame
//
// @param pcap pcap
// [[Rcpp::export]]
DataFrame get_ip_layer(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num; num.reserve(pck_cont.size());
  std::vector<int> tssec; tssec.reserve(pck_cont.size());
  std::vector<int> tsusec; tsusec.reserve(pck_cont.size());
  std::vector<int> sz; sz.reserve(pck_cont.size());
  std::vector<std::string> src; src.reserve(pck_cont.size());
  std::vector<std::string> dst; dst.reserve(pck_cont.size());
  std::vector<int> protocol; protocol.reserve(pck_cont.size());
  std::vector<int> ttl; ttl.reserve(pck_cont.size());
  std::vector<int> dscp; dscp.reserve(pck_cont.size());
  std::vector<int> flags; flags.reserve(pck_cont.size());
  std::vector<int> version; version.reserve(pck_cont.size());
  std::vector<int> fragofs; fragofs.reserve(pck_cont.size());
  std::vector<int> headerlen; headerlen.reserve(pck_cont.size());
  std::vector<int> totallen; totallen.reserve(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;
  timeval ts;

  int i = 0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    IP* ip_layer = (*it_pck)->GetLayer<IP>();

    if (ip_layer) {

      ts = (*it_pck)->GetTimestamp();

      num.push_back(i+1);
      tssec.push_back(ts.tv_sec);
      tsusec.push_back(ts.tv_usec);
      sz.push_back((*it_pck)->GetSize());
      src.push_back(ip_layer->GetSourceIP());
      dst.push_back(ip_layer->GetDestinationIP());
      protocol.push_back((int)ip_layer->GetProtocol());
      ttl.push_back((int)ip_layer->GetTTL());
      dscp.push_back((int)ip_layer->GetDiffServicesCP());
      flags.push_back(ip_layer->GetFlags());
      version.push_back(ip_layer->GetVersion());
      fragofs.push_back(ip_layer->GetFragmentOffset());
      headerlen.push_back(ip_layer->GetHeaderLength());
      totallen.push_back(ip_layer->GetTotalLength());

    }

    i++;

  }

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["src"] = src,
                           _["dst"] = dst,
                           _["protocol"] = protocol,
                           _["size"] = sz,
                           _["header_len"] = headerlen,
                           _["total_len"] = totallen,
                           _["ttl"] = ttl,
                           _["flags"] = flags,
                           _["dscp"] = dscp,
                           _["frag_ofs"] = fragofs,
                           _["stringsAsFactors"] = false);

}


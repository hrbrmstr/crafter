#include <Rcpp.h>
#include <crafter.h>
#include <iostream>

using namespace Rcpp;
using namespace Crafter;

//' @export
//[[Rcpp::export]]
DataFrame read_pcap(std::string capture_file) {

  std::vector<Packet*> pck_cont;

  ReadPcap(&pck_cont, capture_file);

  std::vector<int> num(pck_cont.size());
  std::vector<int> tssec(pck_cont.size());
  std::vector<int> tsusec(pck_cont.size());
  std::vector<int> sz(pck_cont.size());
  std::vector<std::string> src(pck_cont.size());
  std::vector<std::string> dst(pck_cont.size());
  std::vector<int> protocol(pck_cont.size());
  std::vector<int> ttl(pck_cont.size());
  std::vector<int> dscp(pck_cont.size());
  std::vector<int> flags(pck_cont.size());
  std::vector<int> version(pck_cont.size());
  std::vector<int> fragofs(pck_cont.size());
  std::vector<int> headerlen(pck_cont.size());
  std::vector<int> totallen(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;

  int i=0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {

    IP* ip_layer = (*it_pck)->GetLayer<IP>();
    timeval ts = (*it_pck)->GetTimestamp();

    num[i] = i+1;
    tssec[i] = ts.tv_sec;
    tsusec[i] = ts.tv_usec;
    sz[i] = (*it_pck)->GetSize();
    src[i] = ip_layer->GetSourceIP();
    dst[i] = ip_layer->GetDestinationIP();
    protocol[i] = (int)ip_layer->GetProtocol();
    ttl[i] = (int)ip_layer->GetTTL();
    dscp[i] = (int)ip_layer->GetDiffServicesCP();
    flags[i] = ip_layer->GetFlags();
    version[i] = ip_layer->GetVersion();
    fragofs[i] = ip_layer->GetFragmentOffset();
    headerlen[i] = ip_layer->GetHeaderLength();
    totallen[i++] = ip_layer->GetTotalLength();

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



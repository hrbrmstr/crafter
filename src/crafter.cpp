#include <crafter.h>
#include <iostream>
#include <Rcpp.h>

using namespace Rcpp;
using namespace std;
using namespace Crafter;

/* a linked list keeping track of pcaps */
struct node {
  std::vector<Packet*> pcap;
  int link_type ;
  node *next;
};

node pcaplist = *(new node);
node *lstail = &pcaplist;

void pcap_finalizer( std::vector<Packet*>* pcap ) { }

typedef XPtr<std::vector<Packet*>, PreserveStorage, pcap_finalizer> pcapptr;

template<class Seq>
void ReadPcap(Seq* pck_container, int *link_type, const std::string& filename, const std::string& filter = "") {
  /* Handle for the opened pcap session */
  pcap_t *handle;

  OpenOffPcap(link_type,handle,filename,filter);

  /* Prepare the data */
  ThreadReadData<Seq> rd;
  rd.link_type = *link_type;
  rd.pck_container = pck_container;

  u_char* arg = reinterpret_cast<u_char*>(&rd);

  LoopPcap(handle,-1,process_thread<Seq>,arg);

  ClosePcap(handle);

}

// read in a pcap file
//
// @param capture file path to capture file
// [[Rcpp::export]]
pcapptr load_pcap(std::string capture_file, std::string filter) {

  ReadPcap(&(lstail->pcap), &(lstail->link_type), capture_file, filter);
  pcapptr ptr(&(lstail->pcap));
  lstail-> next = new node;
  lstail = lstail->next;
  return(ptr);

}

// get number of packets in capture
//
// @param pcap pcap
// [[Rcpp::export]]
int num_packets(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  if (!pcap)
    throw std::runtime_error("Something is terribly wrong here.");

  return(pcap->size());

}

// Get general packet info (high level) and return a data frame
//
// [[Rcpp::export]]
DataFrame get_packet_info(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap) {

  std::vector<Packet*> pck_cont = *pcap;

  std::vector<int> num(pck_cont.size());
  std::vector<int> tssec(pck_cont.size());
  std::vector<int> tsusec(pck_cont.size());
  std::vector<int> layer_count(pck_cont.size());
  std::vector<std::string> protos(pck_cont.size());
  std::vector<int> packet_size(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;
  timeval ts ;

  int i=0;
  for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {
    num[i] = i+1;
    layer_count[i] = (*it_pck)->GetLayerCount();
    packet_size[i] = (*it_pck)->GetSize();
    ts = (*it_pck)->GetTimestamp();
    tssec[i] = ts.tv_sec;
    tsusec[i] = ts.tv_usec;
    int j = 0;
    protos[i] = "" + (*it_pck)->GetLayer<Layer>(j)->GetName();
    for (j=1; j<layer_count[i]; j++) {
      protos[i] = protos[i] + "," + (*it_pck)->GetLayer<Layer>(j)->GetName();
    }
    i++;
  }

  return DataFrame::create(_["num"] = num,
                           _["tv_sec"] = tssec,
                           _["tv_usec"] = tsusec,
                           _["layer_count"] = layer_count,
                           _["protocols"] = protos,
                           _["packet_size"] = packet_size,
                           _["stringsAsFactors"] = false);


}

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

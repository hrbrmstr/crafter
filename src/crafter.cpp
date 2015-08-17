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

// need this for the Xptr
typedef Crafter::Packet* MyPacket;

void pcap_finalizer( std::vector<Packet*>* pcap ) { };
void pcap_packet_finalizer( MyPacket *pcap_packet ) { };

// this makes it possible to maintain a list of loaded PCAP files and provide
// an interface back to R
typedef XPtr<std::vector<Packet*>, PreserveStorage, pcap_finalizer> pcapptr;

// pointers to specific packets
typedef XPtr<MyPacket, PreserveStorage, pcap_packet_finalizer> pcap_packet_ptr;

// custom (internal) ReadPcap function since we might want the link_type
// available and may be able to optimize some things later on

template<class Seq>
void ReadPcap(Seq* pck_container, int *link_type,
              const std::string& filename, const std::string& filter = "") {
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


// [[Rcpp::export]]
std::vector<std::string> crafter_get_ips(Rcpp::XPtr< std::vector<Crafter::Packet*> > pcap,
                                std::string src_or_dst="src") {

  std::vector<Packet*> pck_cont = *pcap;
  std::vector<std::string> ips; ips.reserve(pck_cont.size());

  std::vector<Packet*>::iterator it_pck;

  if (src_or_dst == "dst") {
    for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {
      IP* ip_layer = (*it_pck)->GetLayer<IP>();
      if (ip_layer) {
        ips.push_back(ip_layer->GetDestinationIP());
      }
    }
  } else {
    for(it_pck = pck_cont.begin() ; it_pck != pck_cont.end() ; it_pck++) {
      IP* ip_layer = (*it_pck)->GetLayer<IP>();
      if (ip_layer) {
        ips.push_back(ip_layer->GetSourceIP());
      }
    }
  }

 return(ips);

}

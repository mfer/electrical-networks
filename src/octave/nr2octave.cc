#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

enum ReturnCodes {
	eOK,
	eInvalidArgs,
	eFileNotFound,
	eInconsistentInput
};

struct edgeName {
	unsigned srcVert, dstVert;
	bool operator<(edgeName const &other) const {
		return (srcVert < other.srcVert) ||
		       ((srcVert == other.srcVert) && (dstVert < other.dstVert));
	}
	bool operator==(edgeName const &other) const {
		return !(*this < other) && !(other < *this);
	}
	edgeName() : srcVert(~0u), dstVert(~0u) {
	}
	edgeName(unsigned sV, unsigned dV) : srcVert(sV), dstVert(dV) {
	}
	
};

typedef map<unsigned,double> vertMap;
typedef map<edgeName,double> edgeMap;
typedef vector<edgeName>  edgeVec;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Uso: " << argv[0] << " infile" << endl << endl;
		return eInvalidArgs;
	}

	ifstream fin(argv[1], ios::in);
	if (!fin.good()) {
		cerr << "Erro: Arquivo '" << argv[1] << "' não encontrado." << endl << endl;
		return eFileNotFound;
	}

	unsigned nVerts, nEdges;
	fin >> nVerts >> nEdges;
	
	edgeMap edges;
	for (unsigned ii = 0; ii < nEdges; ii++) {
		unsigned srcVert, dstVert;
		double cost;
		fin >> srcVert >> dstVert >> cost;
		edges[edgeName(srcVert, dstVert)] = edges[edgeName(dstVert, srcVert)] = cost;
	}
	// Aresta artificial
	edges[edgeName(nVerts, ~0u)] = 0;
	
	edgeVec mappedEdges;
	mappedEdges.reserve(edges.size());
	for (edgeMap::const_iterator it = edges.begin(); it != edges.end(); ++it) {
		mappedEdges.push_back(it->first);
	}

	vertMap vertexMap;
	for (unsigned ii = 0; ii < nVerts; ii++) {
		vertexMap[ii + 1] = 0;
	}
	
	unsigned fileVerts;
	fin >> fileVerts;
	double posFlow = 0.0, negFlow = 0.0;

	for (unsigned ii = 0; ii < fileVerts; ii++) {
		unsigned vert;
		double flow;
		fin >> vert >> flow;
		vertexMap[vert] = flow;
		if (flow > 0.0) {
			posFlow += flow;
		} else if (flow < 0.0) {
			negFlow -= flow;
		}
	}
	
	if (posFlow != negFlow) {
		cerr << "Erro: Fluxo positivo diferente do valor absoluto do fluxo negativo: "
		     << posFlow << " != |-" << negFlow << "|" << endl << endl; 
		return eInconsistentInput;
	}

	cout << "H  = [";
	bool lastEdge = false;
	for (edgeMap::const_iterator mit = edges.begin(); ; ) {
		double cost = mit->second;
		edgeName edge = mit->first;
		++mit;
		lastEdge = mit == edges.end();
		for (edgeVec::const_iterator vit = mappedEdges.begin(); ; ) {
			cout << setw(5);
			if (*vit == edge) {
				cout << cost;
			} else {
				cout << 0;
			}
			++vit;
			if (vit == mappedEdges.end()) {
				if (lastEdge) {
					cout << "];" << endl << endl;
				} else {
					cout << ";" << endl << "      ";
				}
				break;
			} else {
				cout << ", ";
			}
		}
		if (lastEdge) {
			break;
		}
	}
	
	cout << "q  = [";
	for (unsigned ii = 0; ii < mappedEdges.size(); ) {
		cout << setw(5);
		edgeMap::const_iterator it = edges.find(mappedEdges[ii]);
		if (it != edges.end()) {
			if (it->second == 0) {
				cout << 10000;
			} else {
				cout << 0;
			}
		}
		if (++ii == mappedEdges.size()) {
			cout << "];" << endl << endl;
			break;
		} else {
			cout << "; ";
		}
	}

	cout << "lb = [";
	for (unsigned ii = 0; ii < mappedEdges.size(); ) {
		cout << setw(5) << 0;
		if (++ii == mappedEdges.size()) {
			cout << "];" << endl << endl;
			break;
		} else {
			cout << "; ";
		}
	}
	
	cout << "ub = [";
	for (unsigned ii = 0; ii < mappedEdges.size(); ) {
		cout << posFlow;
		if (++ii == mappedEdges.size()) {
			cout << "];" << endl << endl;
			break;
		} else {
			cout << "; ";
		}
	}

	cout << "x0 = [";
	for (unsigned ii = 0; ii < mappedEdges.size(); ) {
		cout << setw(5) << 0;
		if (++ii == mappedEdges.size()) {
			cout << "];" << endl << endl;
			break;
		} else {
			cout << "; ";
		}
	}
	
	cout << "A  = [";
	lastEdge = false;
	for (vertMap::const_iterator vit = vertexMap.begin(); ;) {
		unsigned vert = vit->first;
		++vit;
		lastEdge = vit == vertexMap.end();
		for (edgeMap::const_iterator mit = edges.begin(); ; ) {
			edgeName const edge = mit->first;
			cout << setw(5);
			if (edge.srcVert == vert) {
				cout << 1;
			} else if (edge.dstVert == vert) {
				cout << -1;
			} else {
				cout << 0;
			}
			++mit;
			if (mit == edges.end()) {
				if (lastEdge) {
					cout << "];" << endl << endl;
				} else {
					cout << ";" << endl << "      ";
				}
				break;
			} else {
				cout << ", ";
			}
		}
		if (lastEdge) {
			break;
		}
	}
	
	cout << "b  = [";
	for (vertMap::const_iterator it = vertexMap.begin(); ;) {
		cout << setw(5) << it->second;
		++it;
		if (it == vertexMap.end()) {
			cout << "];" << endl << endl;
			break;
		} else {
			cout << "; ";
		}
	}

	cout << "options=optimset('MaxIter',500); " << endl << endl;
	cout << "[x, obj, info, lambda] = qp(x0, H, q, A, b, lb, ub, options);" << endl;
	cout << "x'" << endl << "obj - x'*q" << endl << "info" << endl << endl;
	
	return eOK;
}

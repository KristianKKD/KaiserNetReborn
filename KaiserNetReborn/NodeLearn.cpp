#include "NodeLayer.h"

extern vector<Layer*> network;
vector<Edge*> RandomUpdate();
void UpdateHeldValues();
double GetError();

int step = 0;

int edges = 0;

void Learn() {
	double oldError = GetError(); //get current error for comparison later

	vector<Edge*> changedEdges = RandomUpdate();
	UpdateHeldValues(); //update output values

	if (step == 0) {
		step++;
		return;
	}

	double newError = GetError(); //get the new error margin

	if (newError > oldError) { //if the new error is larger than the old error, we revert back to the old network
		for (int i = 0; i < changedEdges.size(); i++)
			changedEdges[i]->Revert();
		UpdateHeldValues(); //update the output values again for the next loop
	} else if (step % 100000000)
		Log("NEW IMPROVED ERROR IS: " + to_string(newError));

	step++;
}

vector<Edge*> RandomUpdate() { //random change to all edges
	vector<Edge*> changedEdges = vector<Edge*>();

	for (int i = 1; i < network.size(); i++) //for all layers (not input)
		for (int j = 0; j < network[i]->nodeList.size(); j++) //for all nodes in a layer
			for (int h = 0; h < network[i]->nodeList[j]->outgoingWeights.size(); h++) { //for all outgoing weights
				Edge* e = network[i]->nodeList[j]->outgoingWeights[h];
				e->Update(GetRandomValue());
				changedEdges.push_back(e);
			}

	return changedEdges;
}

void UpdateHeldValues() { //update all nodes in the network other than the input layer, this will mainly change the output values
	for (int i = 1; i < network.size(); i++) //all layers //start at 1 to ignore the input layer
		for (int j = 0; j < network[i]->nodeList.size(); j++) //all nodes
			network[i]->nodeList[j]->UpdateHeldValue(j);
}

double GetError() { //get current error by adding up the different between the input layer nodes and respective output layer nodes
	double resultantError = 0;
	for (int i = 0; i < network[network.size() - 1]->nodeList.size(); i++) {
		double difference = std::abs(network[0]->nodeList[i]->heldValue - network[network.size() - 1]->nodeList[i]->heldValue);
		resultantError += difference;
	}

	return resultantError;
}
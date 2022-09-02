#include "NodeLayer.h"

extern vector<Layer*> network;
vector<Edge*> RandomUpdate();
void UpdateHeldValues();
double GetError();

int step = 0;

int edges = 0;

void Learn() {
	double oldError = GetError();

	vector<Edge*> changedEdges = RandomUpdate();
	UpdateHeldValues();

	if (step == 0) {
		step++;
		return;
	}

	double newError = GetError();

	if (newError > oldError) {
		for (int i = 0; i < changedEdges.size(); i++)
			changedEdges[i]->Revert();
		UpdateHeldValues();
	} else if (step % 100000000)
		Log("NEW IMPROVED ERROR IS: " + to_string(newError));

	step++;
}

vector<Edge*> RandomUpdate() {
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

void UpdateHeldValues() {
	for (int i = 1; i < network.size(); i++)
		for (int j = 0; j < network[i]->nodeList.size(); j++)
			network[i]->nodeList[j]->UpdateHeldValue(j);
}

double GetError() {
	double resultantError = 0;
	for (int i = 0; i < network[network.size() - 1]->nodeList.size(); i++) {
		double difference = network[0]->nodeList[i]->heldValue - network[network.size() - 1]->nodeList[i]->heldValue;
		if (difference < 0)
			difference *= -1;
		resultantError += difference;
	}

	return resultantError;
}
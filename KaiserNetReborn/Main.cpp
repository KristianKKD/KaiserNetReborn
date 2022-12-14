#include "NodeLayer.h"

vector<Layer*> network;
bool pause = true;
bool shuttingDown = false;
string command;

void Learn();
void ReadInput();
void Print();
double GetError();

extern int step;

int main() {
	Log("KaiserNet REBORN version: 0.1.0");
	int hiddenLayers = 5;
	int hiddenNodes = 5;

	vector<double> targets = { 0.2, 0.9, 0.4, 0.1 }; //for now we simply try to recreate the targets that we manually input

	network.push_back(new Layer(targets)); //input layer

	for(int i = 0; i < hiddenLayers; i++) //create the hidden layers
		network.push_back(new Layer(network[i]->nodeList, hiddenNodes));

	network.push_back(new Layer(network[network.size() - 1]->nodeList, (int)targets.size())); //output layer

	thread inputThread = thread(ReadInput);

	Log("Starting main loop, type 'unpause' to begin...");
	while (!shuttingDown) {
		while (pause) {}
		Learn(); //learn by updating the nodes and checking if that has reduced the error margin
	}

	inputThread.join(); //clean up
	inputThread.~thread();

	return Error("Exit success!", 0, true);
}

void ReadInput() {
	while (command != "stop") {
		command = "";
		std::cin >> command;
		if (command == "pause")
			pause = true;
		if (command == "unpause")
			pause = false;
		if (command == "print") {
			pause = true;
			Print();
		}
		if (command == "step") {
			int oldStep = step;
			pause = false;
			while (oldStep == step) {} //wait for the main loop to complete a step
			pause = true;
			Print();
			Log("-----------------------------------------");
		}
	}

	shuttingDown = true;
}

void Print() { //print out detailed information about the current network
	for (int layerIndex = 0; layerIndex < network.size(); layerIndex++) { //layers
		Layer* l = network[layerIndex];
		if(layerIndex == 0)
			Log("INPUT LAYER");
		else if (layerIndex == network.size() - 1)
			Log("OUTPUT LAYER");
		else
			Log("LAYER[" + to_string(layerIndex) + "]");

		for (int nodeIndex = 0; nodeIndex < l->nodeList.size(); nodeIndex++) { //nodes
			Node* n = l->nodeList[nodeIndex];
			Log("	NODE[" + to_string(nodeIndex) + "] VALUE: " +
				to_string(n->heldValue) + "	BIAS: " + to_string(n->bias));

			if (layerIndex != network.size() - 1)
				for (int nodeWeights = 0; nodeWeights < network[layerIndex + 1]->nodeList.size(); nodeWeights++) { //weighted edges
					Edge* e = n->outgoingWeights[nodeWeights];
					Log("		EDGE[" + to_string(nodeWeights) + "]: " + to_string(e->weight));
				}
		}
	}
	Log(" ");

	string outputString = "Targets: ";
	for (int i = 0; i < network[0]->nodeList.size(); i++)
		outputString += to_string(network[0]->nodeList[i]->heldValue) + string((i == network[0]->nodeList.size() - 1) ? "" : ", ");
	
	outputString += " | Outputs:";
	for (int i = 0; i < network[network.size() - 1]->nodeList.size(); i++)
		outputString += to_string(network[network.size() - 1]->nodeList[i]->heldValue) + string((i == network[network.size() - 1]->nodeList.size() - 1) ? "" : ", ");
	
	outputString += " | Error: " + to_string(GetError());

	Log(outputString);
}
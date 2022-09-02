#include "Shared.h"
#include <random>

static double GetRandomValue() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> val(0, 20000);

	double resultValue = (double(val(rng)) - 10000) / 10000.0;//random value between -1 and 1
	if (resultValue == 0)
		return 0.0001;

	return resultValue;
}

static double ActivationFunction(double sum) { //relu
	if (sum > 1)
		sum = 1;
	else if (sum == 0)
		sum = 0.00001;
	else if (sum < -1)
		sum = -1;

	return sum;
}

extern int edges;

class Edge {
private:
	double previousValue = 0;
public:
	double weight;
	int id;

	Edge() {
		weight = GetRandomValue();
		id = edges++;
	}

	void Update(double change) {
		previousValue = weight;
		weight = ActivationFunction(weight + change * LEARNINGRATE);
		Log(to_string(id) + " was: " + to_string(previousValue) + " is now: " + to_string(weight));
	}

	void Revert() {
		Log(to_string(id) + " IS REVERTING FROM: " + to_string(weight) + " TO: " + to_string(previousValue));
		weight = previousValue;
	}
};

class Node {
public:
	double heldValue = 0;
	double bias;
	vector<Node*> incomingNodes;
	vector<Edge*> outgoingWeights;

	Node(double value) {
		heldValue = value;
		bias = 0;
	}

	Node(vector<Node*> previousNodes) {
		bias = GetRandomValue();

		incomingNodes = previousNodes;

		for (int i = 0; i < previousNodes.size(); i++)
			previousNodes[i]->CreateEdge();
	}

	void UpdateHeldValue(int nodeIndex) {
		double result = 0;

		for (int i = 0; i < incomingNodes.size(); i++)
			result += (incomingNodes[i]->GetOutgoingTowards(nodeIndex));

		result += bias;
		heldValue = ActivationFunction(result);
	}

	double GetOutgoingTowards(int nodeID) {
		return heldValue * outgoingWeights[nodeID]->weight;
	}

	void CreateEdge() {
		outgoingWeights.push_back(new Edge());
	}
};

class Layer {
public:
	vector<Node*> nodeList;

	Layer(vector<double> targets) {
		for (int i = 0; i < targets.size(); i++)
			nodeList.push_back(new Node(targets[i]));
	}

	Layer(vector<Node*> previousNodes, int nodeCount) {
		for (int i = 0; i < nodeCount; i++)
			nodeList.push_back(new Node(previousNodes));
	}
};
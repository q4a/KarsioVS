//
// A* path finding algorithm
// created by Rukiya 1/26/2018
// heavily based off of https://github.com/daancode/a-star/blob/master/source/AStar.cpp

#include "AStar.h"
#include <algorithm>

using namespace std::placeholders;

AStar::Node::Node(vec2 aCoordinates, Node *aParent) {
	parent = aParent;
	coordinates = aCoordinates;
	G = H = 0;
}


AStar::uint AStar::Node::getScore() {
	return G + H;
}


// heuristic is euclidean, manhattan, or octagonal (um last two not added)
// square grid + moving diagonally
AStar::Generator::Generator() {
	//setDiagonalMovement(true);
	setHeuristic(&Heuristic::euclidean);
	direction = {
		{0,1}, {1,0}, {0,-1},{-1,0},
		{-1,-1}, {1,1}, {-1,1}, {1,-1}
	};
}

// based on generated map
void AStar::Generator::setWorldSize(vec2 aWorldSize) {
	worldSize = aWorldSize;
}

// square grid, shouldn't need this?
//void AStar::Generator::setDiagonalMovement(bool enable) {
//	directions = (enable ? 8 : 4);
//}


void AStar::Generator::setHeuristic(HeuristicFunction aHeuristic) {
	heuristic = std::bind(aHeuristic, _1, _2); 
}

// unbreakable objects, add another for breakable later?
// based on generated map
void AStar::Generator::addCollision(vec2 coordinates) {
	walls.push_back(coordinates);
}

void AStar::Generator::removeCollision(vec2 coordinates) {
	auto it = std::find(walls.begin(), walls.end(), coordinates);
	if (it != walls.end()) {
		walls.erase(it);
	}
}

void AStar::Generator::clearCollisions() {
	walls.clear();
}


// the actual path finding algorithm
AStar::CoordinateList AStar::Generator::findPath(vec2 source, vec2 target) {
	Node *current = nullptr;
	NodeSet openSet, closedSet;
	openSet.insert(new Node(source));

	while (!openSet.empty()) {
		current = *openSet.begin();
		// for all nodes in openSet, get one with smallest score
		// which is shortest path
		for (auto node : openSet) {
			// if length is the shortest
			if (node->getScore() <= current->getScore()) {
				current = node;	// this will be node to go to
			}
		}
		
		// check first if arrived at destination
		if (current->coordinates == target) {
			break;
		}

		closedSet.insert(current);
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));

		// search each direction
		for (uint i = 0; i < directions; ++i) {
			// get coordinates of each possible direction
			vec2 newCoordinates(current->coordinates + direction[i]);
			// don't consider coordinate if is collision or already on list
			if (detectCollision(newCoordinates) || findNodeOnList(closedSet, newCoordinates)) {
				continue;
			}

			// get total cost
			// diagonal == extra cost
			// if i==4 then diagonal begins
			uint totalCost = current->G + ((i < 4) ? 10 : 14);

			Node *successor = findNodeOnList(openSet, newCoordinates);
			if (successor == nullptr) {
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target);
				openSet.insert(successor);
			}
			// backtrack if all other options too big of cost
			else if (totalCost < successor->G) {
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	// get the ideal path into a list
	CoordinateList path;
	while (current != nullptr) {
		path.push_back(current->coordinates);
		current = current->parent;
	}

	// clean up
	releaseNodes(openSet);
	releaseNodes(closedSet);

	return path;
}

AStar::Node* AStar::Generator::findNodeOnList(NodeSet& aNodes, vec2 aCoordinates) {
	for (auto node : aNodes) {
		if (node->coordinates == aCoordinates) {
			return node;
		}
	}
	return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet& nodes) {
	for (auto it = nodes.begin(); it != nodes.end();) {
		delete *it;
		it = nodes.erase(it);
	}
}

bool AStar::Generator::detectCollision(vec2 coordinates) {
	if (coordinates.x < 0 || coordinates.x >= worldSize.x ||
		coordinates.y < 0 || coordinates.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), coordinates) != walls.end()) {
		return true;
	}
	return false;
}

vec2 AStar::Heuristic::getDelta(vec2 source, vec2 target) {
	return vec2(abs(source.x - target.x), abs(source.y - target.y));
}


AStar::uint AStar::Heuristic::euclidean(vec2 source, vec2 target) {
	auto delta = std::move(getDelta(source, target));
	return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

#include "settings.h"
#include <vector>
#include <utility>

// uselessType to place Snake class inside Grid class
template <typename uselessType>
class Snake {
public:
	Snake();
	~Snake();

	void addSegmentCoords(int x, int y);
	void moveForward(char direction);

	char previousDirection;
	std::vector<std::pair<int, int>> listSegmentCoords;
};

template <typename uselessType>
Snake<uselessType>::Snake() {

	previousDirection = RIGHT;
}

template <typename uselessType>
Snake<uselessType>::~Snake() {

}

template <typename uselessType>
void Snake<uselessType>::addSegmentCoords(int x, int y) {
	listSegmentCoords.push_back(std::make_pair(x, y));
}

template <typename uselessType>
void Snake<uselessType>::moveForward(char direction) {

	if ((direction == UP and previousDirection == DOWN) or
		(direction == DOWN and previousDirection == UP) or
		(direction == RIGHT and previousDirection == LEFT) or
		(direction == LEFT and previousDirection == RIGHT))
		direction = previousDirection;

	//move every snake segment except head
	if (listSegmentCoords.size() > 1)
		for (auto r_it = listSegmentCoords.rbegin(), next = std::next(listSegmentCoords.rbegin(), 1); next != listSegmentCoords.rend(); r_it++, next++) {

			r_it->first = next->first;
			r_it->second = next->second;

		}

	// move head
	switch (direction) {
	case UP:
		listSegmentCoords.begin()->second -= 1;
		break;
	case DOWN:
		listSegmentCoords.begin()->second += 1;
		break;
	case RIGHT:
		listSegmentCoords.begin()->first += 1;
		break;
	case LEFT:
		listSegmentCoords.begin()->first -= 1;
		break;
	default:
		cout << "Wrong moving direction in Snake";
	}

	previousDirection = direction;
}
# include <iostream>
# include <vector>

# define SPLIT_NUM 2
# define MAX_LAYER_NUM 5
# define CUBE_WIDTH 30


#define NUMDIM	3
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2

# define DEBUG_INFO 0

using namespace std;


typedef struct
{
    double x;
    double y;
    double z;
} Point;

typedef struct
{
    Point boxLeft;
    Point boxRight;
} Box;

// typedef struct
// {
//     // Point boxLeft;
//     // Point boxRight;
//     Box *box;
//     vector<BVHNode *> childList;
// } BVHNode;

class BVHNode
{
private:
    /* data */
public:
    Box *box;
    vector<BVHNode *> childList;
    BVHNode(/* args */);
    ~BVHNode();
};

BVHNode::BVHNode(/* args */)
{
    box = new Box;
}

BVHNode::~BVHNode()
{
    delete box;
}


void cpyPoint(Point &src, Point &dst)
{
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}



void getSplit(vector<double> &axis_split, double axis_min, double axis_max)
{
    double distance = axis_max - axis_min;
    double step = distance / SPLIT_NUM;
    axis_split.push_back(axis_min);

    double temp = axis_min;

    for (int i = 0; i < SPLIT_NUM - 1; i++) {
        temp += step;
        axis_split.push_back(temp);
    }

    axis_split.push_back(axis_max);
}

bool ifPointAAllRightB(Point &point_a, Point &point_b)
{
    if (point_a.x < point_b.x) {
        return false;
    }
    else if (point_a.y < point_b.y) {
        return false;
    } 
    else if (point_a.z < point_b.z) {
        return false;
    }
    return true;
}


bool ifBoxContainPoint(Box &box, Point &point)
{
    if (ifPointAAllRightB(point, box.boxLeft)) {
        if (ifPointAAllRightB(box.boxRight, point)) {
            return true;
        }
    }
    return false;
}

void showBox(Box *box)
{
    cout << box->boxLeft.x << "," << box->boxLeft.y << "," << box->boxLeft.z << " ";
    cout << box->boxRight.x << "," << box->boxRight.y << "," << box->boxRight.z << endl;
}

void recurseBuild(BVHNode *thisNode, vector<Point> &pointList, int layer_num)
{
    if (pointList.size() < 2 || layer_num >= MAX_LAYER_NUM) {
        if (DEBUG_INFO) {
            cout << "[INFO] pointlist size: " << pointList.size() << " layer num: " << layer_num << endl;
            cout << "[INFO] end recursing" << endl;
            showBox(thisNode->box);
            cout << "[INFO] leaf node" << endl;
        }

        // leaf node
        for (auto point = pointList.begin(); point != pointList.end(); point++) {
            BVHNode *newNode = new BVHNode;
            newNode->box->boxLeft.x = point->x - CUBE_WIDTH / 2;
            newNode->box->boxLeft.y = point->y - CUBE_WIDTH / 2;
            newNode->box->boxLeft.z = point->z - CUBE_WIDTH / 2;
            newNode->box->boxRight.x = point->x + CUBE_WIDTH / 2;
            newNode->box->boxRight.y = point->y + CUBE_WIDTH / 2;
            newNode->box->boxRight.z = point->z + CUBE_WIDTH / 2;
            thisNode->childList.push_back(newNode);
            if (DEBUG_INFO) showBox(newNode->box);
        }
        return;
    }

    vector<double> x_split;
    vector<double> y_split;
    vector<double> z_split;

    getSplit(x_split, thisNode->box->boxLeft.x, thisNode->box->boxRight.x);
    getSplit(y_split, thisNode->box->boxLeft.y, thisNode->box->boxRight.y);
    getSplit(z_split, thisNode->box->boxLeft.z, thisNode->box->boxRight.z);

    for (int x_idx = 0; x_idx < SPLIT_NUM; x_idx++) {
        for (int y_idx = 0; y_idx < SPLIT_NUM; y_idx++) {
            for (int z_idx = 0; z_idx < SPLIT_NUM; z_idx++) {
                BVHNode *newNode = new BVHNode;
                
                newNode->box->boxLeft.x = x_split[x_idx];
                newNode->box->boxLeft.y = y_split[y_idx];
                newNode->box->boxLeft.z = z_split[z_idx];
                newNode->box->boxRight.x = x_split[x_idx + 1];
                newNode->box->boxRight.y = y_split[y_idx + 1];
                newNode->box->boxRight.z = z_split[z_idx + 1];

                // if (DEBUG_INFO) showBox(newNode->box);

                bool thisBoxContainPoints = false;
                vector<Point> newPointList;
                for (auto point = pointList.begin(); point != pointList.end(); point++) {
                    if (ifBoxContainPoint(*(newNode->box), *point)) {
                        thisBoxContainPoints = true;
                        newPointList.push_back(*point);
                        // if (DEBUG_INFO) cout << point->x << " in box" << endl;
                    }
                }
                if (thisBoxContainPoints) {
                    thisNode->childList.push_back(newNode);
                    recurseBuild(newNode, newPointList, layer_num += 1);
                }
            }
        }
    }
    return;
}

void recurseShow(BVHNode *thisNode, int layer_num)
{
    for (int i = 0; i < layer_num; i++) {
        cout << "  ";
    }
    for (int i = 0; i < layer_num; i++) {
        if (i == 0) cout << "└";
        else cout << "─";
    }
    cout << " ";

    if (thisNode->childList.size() == 0) {
        // leaf node
        cout << "leaf: ";
        showBox(thisNode->box);
        return;
    }

    cout << layer_num << " interval: ";
    showBox(thisNode->box);

    for (auto child = thisNode->childList.begin(); child != thisNode->childList.end(); child++) {
        recurseShow(*child, layer_num + 1);
    }

    return;
}

bool HitBoundingBox(double *minB, double *maxB, double *origin, double *dir, double *coord)
{
    bool inside = true;
	char quadrant[NUMDIM];
	register int i;
	int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[NUMDIM];

	/* Find candidate planes; this loop can be avoided if
   	rays cast all from the eye(assume perpsective view) */
	for (i=0; i<NUMDIM; i++)
		if(origin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}else if (origin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}else	{
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if(inside)	{
		coord = origin;
		return (true);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
		if (quadrant[i] != MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return (false);
	for (i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] *dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return (false);
		} else {
			coord[i] = candidatePlane[i];
		}
	return (true);				/* ray hits box */
}

class BVH
{
private:
    /* data */
public:

    vector<Point> m_pointList;

    int m_cubeSize;

    // Point m_areaLeftPoint;
    // Point m_areaRightPoint;
    Box *sceneBox;

    BVHNode *m_root;
    
    void buildBVHTree();
    void showBVHTree();
    bool hitTraversal(Point &begin, Point &end);

    BVH(int cubeSize, int x_min, int y_min, int z_min, int x_max, int y_max, int z_max);
    ~BVH();
};

bool hitBoundingBoxEntry(Box &box, Point &begin, Point &end)
{   
    double *minA = (double *)malloc(3 * sizeof(double));
    double *maxA = (double *)malloc(3 * sizeof(double));
    double *origin = (double *)malloc(3 * sizeof(double));
    double *dir = (double *)malloc(3 * sizeof(double));
    double *coord = (double *)malloc(3 * sizeof(double));

    minA[0] = box.boxLeft.x;
    minA[1] = box.boxLeft.y;
    minA[2] = box.boxLeft.z;
    maxA[0] = box.boxRight.x;
    maxA[1] = box.boxRight.y;
    maxA[2] = box.boxRight.z;
    origin[0] = begin.x;
    origin[1] = begin.y;
    origin[2] = begin.z;
    dir[0] = end.x - begin.x;
    dir[1] = end.y - begin.y;
    dir[2] = end.z - begin.z;

    return HitBoundingBox(minA, maxA, origin, dir, coord);
}

bool BVH::hitTraversal(Point &begin, Point &end)
{
    bool hit = true;
    BVHNode *node = m_root;
    vector<BVHNode *>tStack;

    if (node == nullptr) return false;

    while (node->childList.size() > 0) {
        // showBox(node->box);
        // cout << "childsize: " << node->childList.size() << endl;
        for (auto childIt = node->childList.begin(); childIt != node->childList.end(); childIt++) {
            // cout << "[CHECK]" << endl;
            // showBox(((*childIt)->box));
            if (hitBoundingBoxEntry(*((*childIt)->box), begin, end)) {
                tStack.push_back(*childIt);
                // cout << "[CHECK] hit" << endl;
            }
            // cout << "[CHECK] done" << endl;
            // cout << endl;
        }
        // cout << "[CHECK] all done" << endl;
        if (tStack.size() == 0) {
            hit = false;
            break;
        }
        node = *(tStack.rbegin());
        tStack.pop_back();
    }

    return hit;
}

void BVH::buildBVHTree()
{
    // 
    BVHNode *newNode = new BVHNode;
    cpyPoint(sceneBox->boxLeft, newNode->box->boxLeft);
    cpyPoint(sceneBox->boxRight, newNode->box->boxRight);

    m_root = newNode;

    recurseBuild(m_root, m_pointList, 0);
}

void BVH::showBVHTree()
{
    recurseShow(m_root, 0);
}

BVH::BVH(int cubeSize, int x_min, int y_min, int z_min, int x_max, int y_max, int z_max)
{
    sceneBox = new Box;
    m_cubeSize = cubeSize;
    sceneBox->boxLeft.x = x_min;
    sceneBox->boxLeft.y = y_min;
    sceneBox->boxLeft.z = z_min;
    sceneBox->boxRight.x = x_max;
    sceneBox->boxRight.y = y_max;
    sceneBox->boxRight.z = z_max;  // TODO: adjust span to suit the real condition
}

BVH::~BVH()
{
    delete sceneBox;
}

void hitTest()
{
    while (true) {
        cout << "input" << endl;

        double *minA = (double *)malloc(3 * sizeof(double));
        double *maxA = (double *)malloc(3 * sizeof(double));
        double *origin = (double *)malloc(3 * sizeof(double));
        double *end = (double *)malloc(3 * sizeof(double));
        double *coord = (double *)malloc(3 * sizeof(double));
        
        for (int i = 0; i < 3; i++) {
            // cin >> minA[i];
            minA[i] = 1.0;
        }
        minA[2] = 0.0;

        for (int i = 0; i < 3; i++) {
            // cin >> maxA[i];
            maxA[i] = 100.0;
        }

        for (int i = 0; i < 3; i++) {
            cin >> origin[i];
        }

        for (int i = 0; i < 3; i++) {
            cin >> end[i];
            end[i] = end[i] - origin[i];
        }

        if (HitBoundingBox(minA, maxA, origin, end, coord)) {
            cout << "Hit!" << endl;
        }
        else {
            cout << "Miss!" << endl;
        }
    }
}

void getPointList(vector<Point> &list, double **array, int n, double x_offset, double y_offset, double z_offset)
{
    for (int i = 0; i < n; i++) {
        Point *newPoint = new Point;
        newPoint->x = array[i][0] + x_offset;
        newPoint->y = array[i][1] + y_offset;
        newPoint->z = array[i][2] + z_offset;
        list.push_back(*newPoint);
    }
}

void swapCoordinates(double **array, int index_a, int index_b, int n)
{
    for(int i = 0; i < n; i++) {
        double temp = array[i][index_a];
        array[i][index_a] = array[i][index_b];
        array[i][index_b] = temp;
    }
}

void buildCubeArray(double **array, const double **cubeArray, const int cubeNumber)
{
    array = (double **)malloc(cubeNumber*sizeof(double*));
    for (int i = 0; i < cubeNumber; i++) {
        array[i] = (double *) malloc(3 * sizeof(double));
        for (int j = 0; j < 3; j++) {
            array[i][j] = cubeArray[i][j];
        }
    }
}

typedef struct
{
    double **cubeArray;
    int cubeNumber;
    double offsetWidth;
} Character;



// int main()
// {
//     cout << "Hello World!" << endl;
//     BVH *test_bvh = new BVH(CUBE_WIDTH, 0,0,0, 20000,20000,20000);


//     // A
//     Character character_A;
//     const int cubeNumber_A = 10;
//     character_A.cubeNumber = cubeNumber_A;
//     // character_A.cubeArray = (double **)malloc(character_A.cubeNumber*sizeof(double*));
//     // for (int i = 0; i < character_A.cubeNumber; i++) {
//     //     character_A.cubeArray[i] = (double *) malloc(3 * sizeof(double));
//     // }
//     const double cubeArray[cubeNumber_A][3] = {
//                             {105,120,30},
//                     {75,90,30},         {135,90,30},
//         {45,60,30},{75,60,30},{105,60,30},{135,60,30},{165,60,30},
//       {30,30,30},                                           {180,30,30}
//     };
//     // character_A.cubeArray = cubeArray;
//     buildCubeArray(character_A.cubeArray, cubeArray, cubeNumber_A);
//     character_A.offsetWidth = 210;

//     Character character_D;
//     const int cubeNumber_D = 9;
//     character_D.cubeNumber = cubeNumber_D;
//     const 

// }


int main()
{
    // cout << "Hello World!" << endl;
    BVH *test_bvh = new BVH(CUBE_WIDTH, 0,0,0, 20000,20000,20000);

    // int numList[] = {105, 120, 30, 75, 90, 30, 135, 90, 30, 45, 60, 30, 75, 60, 30, 105, 60, 30, 135, 60, 30, 165, 60, 30, 30, 30, 30, 180, 30, 30, 240, 150, 30, 240, 120, 30, 240, 90, 30, 240, 60, 30, 240, 30, 30, 270, 90, 30, 300, 90, 30, 330, 150, 30, 330, 120, 30, 330, 90, 30, 330, 60, 30, 330, 30, 30, 390, 150, 30, 390, 120, 30, 390, 90, 30, 390, 60, 30, 390, 30, 30, 390, 90, 60, 390, 90, 90, 390, 150, 120, 390, 120, 120, 390, 90, 120, 390, 60, 120, 390, 30, 120, 540, 150, 30, 540, 120, 30, 540, 90, 30, 540, 60, 30, 540, 30, 30, 570, 30, 30, 600, 30, 30, 660, 150, 30, 660, 120, 30, 660, 90, 30, 660, 60, 30, 660, 30, 30, 660, 150, 60, 660, 90, 60, 660, 150, 90, 660, 90, 90, 780, 30, 120, 780, 30, 60, 810, 30, 30, 810, 30, 60, 810, 30, 90, 810, 30, 120, 810, 30, 150, 840, 30, 120, 840, 30, 60, 870, 30, 150, 870, 30, 120, 870, 30, 90, 870, 30, 60, 870, 30, 30, 900, 30, 120, 900, 30, 60, 1035, 120, 30, 1005, 90, 30, 1065, 90, 30, 975, 60, 30, 1005, 60, 30, 1035, 60, 30, 1065, 60, 30, 1095, 60, 30, 960, 30, 30, 1110, 30, 30, 1170, 120, 30, 1170, 30, 30, 1200, 120, 30, 1200, 90, 30, 1200, 60, 30, 1200, 30, 30, 1230, 120, 30, 1230, 30, 30, 1290, 90, 30, 1320, 120, 30, 1320, 90, 30, 1320, 60, 30, 1320, 30, 30, 1350, 90, 30, 1350, 30, 30, 1410, 120, 30, 1410, 90, 30, 1410, 60, 30, 1410, 30, 30, 1440, 120, 30, 1440, 30, 30, 1470, 105, 30, 1485, 75, 30, 1470, 45, 30, 1530, 150, 30, 1530, 120, 30, 1530, 90, 30, 1530, 60, 30, 1530, 30, 30, 1530, 150, 60, 1530, 90, 60, 1530, 150, 90, 1530, 90, 90, 1650, 150, 30, 1650, 120, 30, 1650, 90, 30, 1650, 60, 30, 1650, 30, 30, 1680, 30, 30, 1710, 30, 30, 1770, 150, 30, 1770, 120, 30, 1770, 90, 30, 1770, 60, 30, 1770, 30, 30, 1770, 150, 60, 1770, 90, 60, 1770, 150, 90, 1770, 90, 90, 1890, 120, 30, 1890, 90, 30, 1890, 60, 30, 1890, 30, 30, 1920, 120, 30, 1920, 30, 30, 1950, 105, 30, 1965, 75, 30, 1950, 45, 30};
    int numList[] = {
        105, 120, 30, 75, 90, 30, 135, 90, 30, 45, 60, 30, 75, 60, 30, 105, 60, 30, 135, 60, 30, 165, 60, 30, 30, 30, 30, 180, 30, 30, 240, 150, 30, 240, 120, 30, 240, 90, 30, 240, 60, 30, 240, 30, 30, 270, 90, 30, 300, 90, 30, 330, 150, 30, 330, 120, 30, 330, 90, 30, 330, 60, 30, 330, 30, 30, 390, 150, 30, 390, 120, 30, 390, 90, 30, 390, 60, 30, 390, 30, 30, 390, 90, 60, 390, 90, 90, 390, 150, 120, 390, 120, 120, 390, 90, 120, 390, 60, 120, 390, 30, 120, 540, 150, 30, 540, 120, 30, 540, 90, 30, 540, 60, 30, 540, 30, 30, 570, 30, 30, 600, 30, 30, 660, 150, 30, 660, 120, 30, 660, 90, 30, 660, 60, 30, 660, 30, 30, 660, 150, 60, 660, 90, 60, 660, 150, 90, 660, 90, 90, 780, 30, 120, 780, 30, 60, 810, 30, 30, 810, 30, 60, 810, 30, 90, 810, 30, 120, 810, 30, 150, 840, 30, 120, 840, 30, 60, 870, 30, 150, 870, 30, 120, 870, 30, 90, 870, 30, 60, 870, 30, 30, 900, 30, 120, 900, 30, 60, 1035, 120, 30, 1005, 90, 30, 1065, 90, 30, 975, 60, 30, 1005, 60, 30, 1035, 60, 30, 1065, 60, 30, 1095, 60, 30, 960, 30, 30, 1110, 30, 30, 1170, 120, 30, 1170, 30, 30, 1200, 120, 30, 1200, 90, 30, 1200, 60, 30, 1200, 30, 30, 1230, 120, 30, 1230, 30, 30, 1290, 90, 30, 1320, 120, 30, 1320, 90, 30, 1320, 60, 30, 1320, 30, 30, 1350, 90, 30, 1350, 30, 30, 1410, 120, 30, 1410, 90, 30, 1410, 60, 30, 1410, 30, 30, 1440, 120, 30, 1440, 30, 30, 1470, 105, 30, 1485, 75, 30, 1470, 45, 30, 1530, 150, 30, 1530, 120, 30, 1530, 90, 30, 1530, 60, 30, 1530, 30, 30, 1530, 150, 60, 1530, 90, 60, 1530, 30, 60, 1530, 150, 90, 1530, 90, 90, 1530, 30, 90, 1650, 150, 30, 1650, 120, 30, 1650, 90, 30, 1650, 60, 30, 1650, 30, 30, 1680, 30, 30, 1710, 30, 30, 1770, 150, 30, 1770, 120, 30, 1770, 90, 30, 1770, 60, 30, 1770, 30, 30, 1770, 150, 60, 1770, 90, 60, 1770, 150, 90, 1770, 90, 90, 1890, 120, 30, 1890, 90, 30, 1890, 60, 30, 1890, 30, 30, 1920, 120, 30, 1920, 30, 30, 1950, 105, 30, 1965, 75, 30, 1950, 45, 30
    };

    int num = 408;
    // for (int i = 0; i < 20; i++) {
    //     Point *newPoint = new Point;
    //     newPoint->x = i * 900;
    //     newPoint->y = i * 900;
    //     newPoint->z = i * 900;
    //     test_bvh->m_pointList.push_back(*newPoint);
    // }

    for (int i = 0; i < num/3; i++) {
        Point *newPoint = new Point;
        newPoint->x = numList[i*3];
        newPoint->y = numList[i*3+1];
        newPoint->z = numList[i*3+2];
        test_bvh->m_pointList.push_back(*newPoint);
    }

    test_bvh->buildBVHTree();
    // test_bvh->showBVHTree();

    cout << "Hello! ";

    while(true) {
        Point *s = new Point;
        Point *e = new Point;
        cout << "where will your ray start?" << endl;
        cout << "Start x:" << endl;
        cin >> s->x;
        cout << "Start y:" << endl;
        cin >> s->y;
        cout << "Start z:" << endl;
        cin >> s->z;

        cout << "and where will your ray go?" << endl;
        cout << "Point x:" << endl;
        cin >> e->x;
        cout << "Point y:" << endl;
        cin >> e->y;
        cout << "Point z:" << endl;
        cin >> e->z;

        if (test_bvh->hitTraversal(*s, *e)) {
            cout << "Hit!" << endl;
        }
        else {
            cout << "Miss!" << endl;
        }
    }

    // Point *s = new Point;
    // s->x = 7185;
    // s->y = 7200;
    // s->z = 7200;

    // Point *e = new Point;
    // e->x = 7300;
    // e->y = 7200;
    // e->z = 7200;

    // s->x = 7185;
    // s->y = 7300;
    // s->z = 7200;

    // e->x = 7300;
    // e->y = 7300;
    // e->z = 7200;

    // if (test_bvh->hitTraversal(*s, *e)) {
    //     cout << "Hit!" << endl;
    // }
    // else {
    //     cout << "Miss!" << endl;
    // }
    // // hitTest();
}
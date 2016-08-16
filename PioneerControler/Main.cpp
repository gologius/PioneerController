/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
This source is referenced ARIA example.
*/

#include "Aria.h"
#include "windows.h"

class ConnHandler
{
public:
	// Constructor
	ConnHandler(ArRobot *robot);
	// Destructor, its just empty
	~ConnHandler(void) {}
	// to be called if the connection was made
	void connected(void);
	// to call if the connection failed
	void connFail(void);
	// to be called if the connection was lost
	void disconnected(void);
protected:
	// robot pointer
	ArRobot *myRobot;
	// the functor callbacks
	ArFunctorC<ConnHandler> myConnectedCB;
	ArFunctorC<ConnHandler> myConnFailCB;
	ArFunctorC<ConnHandler> myDisconnectedCB;
};

ConnHandler::ConnHandler(ArRobot *robot) :
myConnectedCB(this, &ConnHandler::connected),
myConnFailCB(this, &ConnHandler::connFail),
myDisconnectedCB(this, &ConnHandler::disconnected)

{
	myRobot = robot;
	myRobot->addConnectCB(&myConnectedCB, ArListPos::FIRST);
	myRobot->addFailedConnectCB(&myConnFailCB, ArListPos::FIRST);
	myRobot->addDisconnectNormallyCB(&myDisconnectedCB, ArListPos::FIRST);
	myRobot->addDisconnectOnErrorCB(&myDisconnectedCB, ArListPos::FIRST);
}

// just exit if the connection failed
void ConnHandler::connFail(void)
{
	printf("directMotionDemo connection handler: Failed to connect.\n");
	myRobot->stopRunning();
	Aria::exit(1);
	return;
}

// turn on motors, and off sonar, and off amigobot sounds, when connected
void ConnHandler::connected(void)
{
	printf("directMotionDemo connection handler: Connected\n");
	myRobot->comInt(ArCommands::SONAR, 0);
	myRobot->comInt(ArCommands::ENABLE, 1);
	myRobot->comInt(ArCommands::SOUNDTOG, 0);
}

// lost connection, so just exit
void ConnHandler::disconnected(void)
{
	printf("directMotionDemo connection handler: Lost connection, exiting program.\n");
	Aria::exit(0);
}

/*
引数の単位は[mm]
setvel() 両輪の速度を一括設定する
setVel2() 左，右の車輪の速度を別々に設定する
setRotVel() 旋回速度の設定

move() 指定された距離だけ前方(後方)に移動する
setHeading() 指定された角度だけ右(左)に旋回する

*/

//指定した方向に移動する
void test1(ArRobot& robot) {

	double verticalVel = 500.0;
	double turnVel = 100.0;

	while (true){

		if (GetAsyncKeyState(VK_UP)){
			printf("forward\n");
			robot.lock();
			robot.setVel(verticalVel);
			robot.setRotVel(0.0);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_DOWN)){
			printf("back\n");
			robot.lock();
			robot.setVel(-verticalVel);
			robot.setRotVel(0.0);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_LEFT)){
			printf("left turn\n");
			robot.lock();
			robot.setVel(0.0);
			robot.setRotVel(turnVel);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_RIGHT)){
			printf("right turn\n");
			robot.lock();
			robot.setVel(0.0);
			robot.setRotVel(-turnVel);
			robot.unlock();
		}
		
		if (GetAsyncKeyState(VK_SPACE)){
			printf("stop\n");
			robot.lock();
			robot.setVel(0.0);
			robot.setRotVel(0.0);
			robot.unlock();
		}

		if (GetAsyncKeyState(VK_ESCAPE)){

			break;
		}

		ArUtil::sleep(50);
	} // while

}

//一定の角度，方向ごとに動く
void test2(ArRobot& robot) {

	double nowDeg = 0.0;
	double verticalDist = 500.0;
	double turnDeg = 45.0;

	while (true){

		if (GetAsyncKeyState(VK_UP) && robot.isMoveDone()){
			printf("forward\n");
			robot.lock();
			robot.move(verticalDist);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_DOWN) && robot.isMoveDone()){
			printf("back\n");
			robot.lock();
			robot.move(-verticalDist);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_LEFT) && robot.isHeadingDone()){
			printf("left turn\n");
			nowDeg += turnDeg;
			robot.lock();
			robot.setHeading(nowDeg);
			robot.unlock();
		}
		else if (GetAsyncKeyState(VK_RIGHT) && robot.isHeadingDone()){
			printf("right turn\n");
			nowDeg -= turnDeg;
			robot.lock();
			robot.setHeading(nowDeg);
			robot.unlock();
		}

		if (GetAsyncKeyState(VK_SPACE)){
			printf("stop\n");
			robot.lock();
			robot.setVel(0.0);
			robot.setRotVel(0.0);
			robot.stop();
			robot.unlock();
		}

		if (GetAsyncKeyState(VK_ESCAPE)){

			break;
		}

		ArUtil::sleep(50);
	} // while

}

int main()
{
	//引数用意
	int argc = 4;
	char** argv = new char*[argc];
	for (int i = 0; i < argc; i++)
		argv[i] = new char[128];
	argv[0] = "-robotPort";
	argv[1] = "COM8";
	argv[2] = "-robotBaud";
	argv[3] = "9600";

	Aria::init();
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();
	ArRobot robot;
	ArRobotConnector con(&argParser, &robot);
	ConnHandler ch(&robot);

	if (!Aria::parseArgs())
	{
		Aria::logOptions();
		Aria::exit(1);
		return 1;
	}

	if (!con.connectRobot())
	{
		ArLog::log(ArLog::Normal, "directMotionExample: Could not connect to the robot. Exiting.");

		if (argParser.checkHelpAndWarnUnparsed())
		{
			Aria::logOptions();
		}
		Aria::exit(1);
		return 1;
	}

	ArLog::log(ArLog::Normal, "directMotionExample: Connected.");

	if (!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		Aria::exit(1);
	}

	robot.runAsync(false);

	//test1(robot);
	test2(robot);

	Aria::exit(0);
	return 0;
}



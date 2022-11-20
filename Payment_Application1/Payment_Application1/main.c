#include <stdio.h>
#include <string.h>

#include "CARD/Card.h"
#include "TERMINAL/Terminal.h"
#include "SERVER/Server.h"
#include "TEST/TEST.h"
#include "APP/app.h"

int main()
{
	TEST_Start();
	appStart();
	return 0;
}
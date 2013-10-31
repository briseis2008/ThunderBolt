//
//  Plane.cpp
//  ThunderBolt
//
//  Created by yigu on 10/31/13.
//  Copyright (c) 2013 CMU. All rights reserved.
//

#include "Plane.h"


void Plane::Move(int key){
    if(FSKEY_LEFT==key)
	{
		position.x -= 10;
	}
	else if(FSKEY_RIGHT==key)
	{
		position.x += 10;
	}
    else if(FSKEY_UP==key)
    {
        position.y += 10;
    }
    else{
        position.y -=10;
    }
}
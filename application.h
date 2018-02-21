/*
 * File: application.h
 * Authors: Alexander Epp, Mitchell Epp
 * Project: CMPUT274 Final Project
 * Description: Includes Application type appropriate for the platform.
 */

// ILI9341Framework and SDLFramework both implement the WindowFramework
// interface, so Application may be used as an implementation of
// WindowFramework without knowledge of which class it actually is.

#ifdef ARDUINO_BUILD
#include "ILI9341framework.h"
typedef ILI9341Framework Application;
#else
#include "sdlframework.h"
typedef SDLFramework Application;
#endif
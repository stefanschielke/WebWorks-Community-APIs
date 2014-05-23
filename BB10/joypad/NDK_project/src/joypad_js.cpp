/*
 * Copyright 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include "../public/tokenizer.h"
#include "joypad_js.hpp"
#include "joypad_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
joypadJS::joypadJS(const std::string& id) :
		m_id(id) {
    m_pLogger = new webworks::Logger("Joypad", this);
	m_pjoypadController = new webworks::joypadNDK(this);
}

/**
 * TemplateJS destructor.
 */
joypadJS::~joypadJS() {
	if (m_pjoypadController)
		delete m_pjoypadController;
	if (m_pLogger)
	    delete m_pLogger;
}

webworks::Logger* joypadJS::getLog() {
    return m_pLogger;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "joypadJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the TemplateJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "joypadJS") {
		return new joypadJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool joypadJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string joypadJS::InvokeMethod(const string& command) {
	// command appears with parameters following after a space
	int index = command.find_first_of(" ");
	std::string strCommand = command.substr(0, index);
	std::string arg = command.substr(index + 1, command.length());

	// based on the command given, run the appropriate method in template_ndk.cpp
	if (strCommand == "start") {
		return m_pjoypadController->start();
	} else if (strCommand == "stop") {
		return m_pjoypadController->stop();
	}

	strCommand.append(";");
	strCommand.append(command);
	return strCommand;
}

// Notifies JavaScript of an event
void joypadJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}

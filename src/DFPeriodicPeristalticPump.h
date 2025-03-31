 /*
* This file and associated .cpp file are licensed under the GPLv3 License Copyright (c) 2024 Sam Groveman
* 
* DFRobot Peristaltic Pump: https://www.dfrobot.com/product-1698.html
*
* Contributors: Sam Groveman
*/

#pragma once
#include <Arduino.h>
#include <DFPeristalticPump.h>
#include <PeriodicTask.h>

/// @brief Adds auto triggering to DFPeristalticPump
class DFPeriodicPeristalticPump : public DFPeristalticPump, public PeriodicTask {
	protected:
		/// @brief Additional settings required
		struct {
			/// @brief Enables the task
			bool enabled = false;
		} add_config;

		JsonDocument addAdditionalConfig();
	public:
		DFPeriodicPeristalticPump(String Name, int Pin, String ConfigFile = "DFPeriodPump.json");
		bool begin();
		void runTask(ulong elapsed);
		String getConfig();
		bool setConfig(String config, bool save);
};

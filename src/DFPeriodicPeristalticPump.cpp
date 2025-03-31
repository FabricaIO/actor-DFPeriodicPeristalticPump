#include "DFPeriodicPeristalticPump.h"

/// @brief Creates a peristaltic pump object
/// @param Name The device name
/// @param Pin The pin to use
/// @param ConfigFile The file name to store settings in
DFPeriodicPeristalticPump::DFPeriodicPeristalticPump(String Name, int Pin, String ConfigFile) : DFPeristalticPump (Name, Pin, ConfigFile) {}

/// @brief Starts a automatic pump
/// @return True on success
bool DFPeriodicPeristalticPump::begin() {
	bool result = false;
	bool configExists = checkConfig(config_path);
	// Create settings directory if necessary
	if (DFPeristalticPump::begin()) {
		// Set description
		Description.type = "pump";
		Description.name = "Periodic Peristaltic Pump";
		if (!configExists) {
			// Set defaults
			task_config.taskName = "Period Pump";
			task_config.taskPeriod = 1000;
			result = setConfig(getConfig(), true);
		} else {
			// Load settings
			result = setConfig(Storage::readFile(config_path), false);
		}
	}
	return result;
}

/// @brief Runs the auto pump task
/// @param elapsed The amount of time, in ms, since this was last called
void DFPeriodicPeristalticPump::runTask(ulong elapsed) {
	if (taskPeriodTriggered(elapsed)) {
		dose();
	}
}

/// @brief Gets the current config
/// @return A JSON string of the config
String DFPeriodicPeristalticPump::getConfig() {
	JsonDocument doc = addAdditionalConfig();
	// Create string to hold output
	String output;
	// Serialize to string
	serializeJson(doc, output);
	return output;
}

/// @brief Sets the configuration for this device
/// @param config A JSON string of the configuration settings
/// @param save If the configuration should be saved to a file
/// @return True on success
bool DFPeriodicPeristalticPump::setConfig(String config, bool save) {
	if (DFPeristalticPump::setConfig(config, false)) {
		// Allocate the JSON document
		JsonDocument doc;
		// Deserialize file contents
		DeserializationError error = deserializeJson(doc, config);
		// Test if parsing succeeds.
		if (error) {
			Logger.print(F("Deserialization failed: "));
			Logger.println(error.f_str());
			return false;
		}
		// Assign loaded values
		add_config.enabled = doc["Enabled"].as<bool>();
		task_config.taskName = doc["taskName"].as<std::string>();
		task_config.taskPeriod = doc["taskPeriod"].as<ulong>() * 60000; // Convert to milliseconds
		if (save) {
			if (!saveConfig(config_path, config)) {
				return false;
			}
		}
		return enableTask(add_config.enabled);
	}
	return false;
}

/// @brief Collects all the base class parameters and additional parameters
/// @return a JSON document with all the parameters
JsonDocument DFPeriodicPeristalticPump::addAdditionalConfig() {
	// Allocate the JSON document
  	JsonDocument doc;
	// Deserialize file contents
	DeserializationError error = deserializeJson(doc, DFPeristalticPump::getConfig());
	// Test if parsing succeeds.
	if (error) {
		Logger.print(F("Deserialization failed: "));
		Logger.println(error.f_str());
		return doc;
	}
	doc["Enabled"] = add_config.enabled;
	doc["taskName"] = task_config.taskName;
	doc["taskPeriod"] = task_config.taskPeriod / 60000; // Convert to minutes
	return doc;
}
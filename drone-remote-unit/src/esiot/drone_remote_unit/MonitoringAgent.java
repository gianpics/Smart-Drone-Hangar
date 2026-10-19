package esiot.drone_remote_unit;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	// Messages coming in format: <droneState;hangarState;distance>
	// distance parameter can be empty

	public MonitoringAgent(SerialCommChannel channel, DashboardView view, LogView log) throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}

	public void run() {
		while (true) {
			try {
				String msg = channel.receiveMsg();
				logger.log("raw: " + msg);

				// check format
				if (msg.startsWith("<") && msg.endsWith(">")) {
					parseArduinoMsg(msg);
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
	}

	// Message parsing and UI update
	private void parseArduinoMsg(String msg) {
		try {
			String content = msg.substring(1, msg.length() - 1);
			String[] parts = content.split(";", -1); // limit = -1 to preserve empty parameter

			if (parts.length == 3) {
				String droneState  = parts[0];
				String hangarState = parts[1];
				String distance    = parts[2].isEmpty() ? "--" : parts[2] + " m";

				view.updateControls(droneState, hangarState, distance);

				//logger.log("Drone: " + droneState + " | Hangar: " + hangarState + " | Dist: " + distance);
			} else {
				logger.log("[WARN] Invalid message format: " + msg);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
			logger.log("[ERROR] Parsing error: " + msg);
		}
	}
}

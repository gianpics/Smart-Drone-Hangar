package esiot.drone_remote_unit;

public class DashboardController {

	SerialCommChannel channel; // can be null if port is not found
	DashboardView view;
	LogView logger;

	public DashboardController(String port, DashboardView view, LogView logger) {
		this.view = view;
		this.logger = logger;

		// Launch dashboard even in case serial port is not found
		try {
			channel = new SerialCommChannel(port, 115200);
			new MonitoringAgent(channel, view, logger).start();

			System.out.println("Waiting Arduino for rebooting...");
			Thread.sleep(4000);
			System.out.println("Ready.");
		} catch (Exception ex) {
			System.out.println("[WARNING] Serial port '" + port + "' not found or not available.");
			System.out.println("[WARNING] Dashboard will open without serial communication.");
			channel = null;
		}
	}

	// Send TAKE_OFF command on serial
	public void sendTakeOff() {
		if (channel != null) {
			channel.sendMsg("TAKE_OFF");
		} else {
			System.out.println("[WARNING] No serial port.");
		}
	}

	// Send LANDING command on serial
	public void sendLanding() {
		if (channel != null) {
			channel.sendMsg("LANDING");
		} else {
			System.out.println("[WARNING] No serial port.");
		}
	}
}

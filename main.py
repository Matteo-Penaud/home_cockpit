import signal
import sys
import time

from SimConnect import AircraftRequests

from pc.msfs.interface import MsfsThread

AIRCRAFT_REQS = {
    "COM_ACTIVE_FREQUENCY:1": None,
    "COM_STANDBY_FREQUENCY:1": None,
    "PLANE_ALTITUDE": None,
}


def signal_handler(sig: int, frame) -> None:
    sys.exit(-2)


def main() -> int:
    simconnect_thread = MsfsThread()
    aircraft_req = AircraftRequests(simconnect_thread.interface.handler, 100)
    for req in AIRCRAFT_REQS:
        req_data = req.split(":")[0]
        aircraft_req.find(req_data)
    simconnect_thread.start()

    while not simconnect_thread.is_connected.is_set():
        if not simconnect_thread.is_alive():
            return -1

    while simconnect_thread.is_connected.is_set():
        simconnect_thread.interface.read_datas(aircraft_req, AIRCRAFT_REQS)
        print(AIRCRAFT_REQS)
        time.sleep(0.1)

    return 0


if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)

    sys.exit(main())

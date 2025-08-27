import logging
import threading
from abc import ABC, abstractmethod
from typing import Any

from SimConnect import Request, SimConnect

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class MsfsInterface(ABC):
    @abstractmethod
    def connect(self) -> None:
        pass

    @abstractmethod
    def disconnect(self) -> None:
        pass

    @abstractmethod
    def read_datas(self, request: Request, datas: dict[str, Any]) -> None:
        pass


class MsfsMock(MsfsInterface):
    def connect(self) -> None:
        self.handler = True

    def disconnect(self) -> None:
        self.handler = False

    def read_datas(self, request: Request, datas: dict[str, Any]) -> None:
        return super().read_datas(datas)


class MsfsSimconnect(MsfsInterface):
    def __init__(self) -> None:
        super().__init__()

        self.handler: SimConnect = SimConnect(auto_connect=False)
        self.requests: list[Any]

    def connect(self) -> None:
        self.handler.connect()

    def disconnect(self) -> None:
        self.handler.exit()

    def read_datas(self, request: Request, datas: dict[str, Any]) -> None:
        for data in datas:
            datas[data] = request.get(data)


class MsfsThread(threading.Thread):
    def __init__(self) -> None:
        super().__init__()
        self.is_connected: threading.Event = threading.Event()
        self.daemon = True
        self.interface = MsfsSimconnect()

    def run(self):
        try:
            self.interface.connect()
        except ConnectionError:
            logger.exception("SimConnect thread not started :")
            self.is_connected.clear()
            raise
        else:
            logger.info("Successfully created SimConnect thread.")
            self.is_connected.set()

        while self.is_connected.is_set():
            pass

    def stop(self):
        self.is_connected.clear()
        self.interface.disconnect()

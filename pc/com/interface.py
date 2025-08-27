import queue
from abc import ABC, abstractmethod


class SerialInterface(ABC):
    @abstractmethod
    def open(self, port: str, baudrate: int) -> None:
        pass

    @abstractmethod
    def close(self) -> bytes | None:
        pass

    @abstractmethod
    def write(self, datas: bytes) -> None:
        pass

    @abstractmethod
    def read(self) -> bytes | None:
        pass


class SerialMockt(SerialInterface):
    def __init__(self) -> None:
        super().__init__()

        self.rx_queue = queue.Queue()
        self.tx_queue = queue.Queue()
        self.is_open = False

    def open(self, port: str, baudrate: int) -> None:
        self.is_open = True

    def close(self) -> bytes | None:
        self.is_open = False

    def write(self, datas: bytes) -> None:
        self.tx_queue.put(datas)

    def read(self) -> bytes | None:
        try:
            return self.tx_queue.get_nowait()
        except queue.Empty:
            return None

    def inject_rx(self):
        pass

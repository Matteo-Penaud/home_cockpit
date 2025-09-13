"""Packets management."""

START_BYTE = 0xAA
STOP_BYTE = 0x55


def compute_checksum(data: bytes) -> int:
    try:
        result = 0
        for b in data:
            result ^= b
    except TypeError as e:
        msg = "Input data should be bytes"
        raise TypeError(msg) from e

    return result


def encode(req_id: int, data: bytes) -> bytes:
    frame = [START_BYTE, req_id, len(data), *list(data), 0x00, STOP_BYTE]
    frame[-2] = compute_checksum(frame)

    return bytes(frame)


def decode(frame: bytes) -> tuple[int, bytes]:
    # Remove checksum to check its validity
    frame_without_checksum = list(frame)
    frame_without_checksum[-2] = 0x00

    if frame[0] != START_BYTE:
        msg = "Invalid start byte"
        raise ValueError(msg)
    if frame[-1] != STOP_BYTE:
        msg = "Invalid stop byte"
        raise ValueError(msg)
    if frame[-2] != compute_checksum(frame_without_checksum):
        msg = "Invalid checksum byte"
        raise ValueError(msg)
    return (frame[1], bytes(frame[3:-2]))

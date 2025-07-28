START_BYTE = 0xAA
STOP_BYTE = 0x55


def compute_checksum(data: bytes) -> int:
    try:
        result = 0
        for b in data:
            result ^= b
    except TypeError:
        raise AssertionError("Input data should be bytes")

    return result


def encode(req_id: int, data: bytes) -> bytes:
    frame = [START_BYTE, req_id, len(data)] + [x for x in data] + [0x00, STOP_BYTE]
    frame[-2] = compute_checksum(frame)

    return bytes(frame)

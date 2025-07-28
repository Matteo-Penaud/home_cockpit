from protocol.packet import compute_checksum, START_BYTE, STOP_BYTE, encode
import pytest
import random


def test_compute_checksum_single_byte():
    assert compute_checksum(bytes([0x11])) == 0x11


def test_compute_checksum_multiple_bytes():
    data = bytes(range(10))
    result = 0
    for b in data:
        result ^= b

    assert compute_checksum(data) == result


def test_compute_checksum_empty():
    assert compute_checksum(b"") == 0


def test_compute_checksum_invalid_value():
    with pytest.raises(AssertionError):
        compute_checksum("Bonjour")

    with pytest.raises(AssertionError):
        compute_checksum(85)


def test_encode_single_byte():
    expected_frame_raw = [START_BYTE, 0x00, 0x01, 0xBB, 0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0, bytes([0xBB])) == bytes(expected_frame_raw)


def test_encode_multiple_bytes():
    random_data = [b for b in random.randbytes(0x10)]
    expected_frame_raw = [START_BYTE, 0xCA, 0x10] + random_data + [0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0xCA, bytes(random_data)) == bytes(expected_frame_raw)


def test_encode_empty_data():
    expected_frame_raw = [START_BYTE, 0xFE, 0x00, 0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0xFE, bytes()) == bytes(expected_frame_raw)


def test_encode_invalid_req_id():
    with pytest.raises(ValueError):
        encode(0x100, bytes())

"""Tests for packet.py."""

import random

import pytest
from com.packet import START_BYTE, STOP_BYTE, compute_checksum, decode, encode


def test_compute_checksum_single_byte() -> None:
    assert compute_checksum(bytes([0x11])) == 0x11  # noqa: PLR2004


def test_compute_checksum_multiple_bytes() -> None:
    data = bytes(range(10))
    result = 0
    for b in data:
        result ^= b

    assert compute_checksum(data) == result


def test_compute_checksum_empty() -> None:
    assert compute_checksum(b"") == 0


def test_compute_checksum_invalid_value() -> None:
    with pytest.raises(TypeError, match="Input data should be bytes"):
        compute_checksum("Bonjour")

    with pytest.raises(TypeError):
        compute_checksum(85, match="Input data should be bytes")


def test_encode_single_byte() -> None:
    expected_frame_raw = [START_BYTE, 0x00, 0x01, 0xBB, 0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0, bytes([0xBB])) == bytes(expected_frame_raw)


def test_encode_multiple_bytes() -> None:
    random_data = list(random.randbytes(0x10))  # noqa: S311
    expected_frame_raw = [START_BYTE, 0xCA, 0x10, *random_data, 0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0xCA, bytes(random_data)) == bytes(expected_frame_raw)


def test_encode_empty_data() -> None:
    expected_frame_raw = [START_BYTE, 0xFE, 0x00, 0x00, STOP_BYTE]
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))

    assert encode(0xFE, b"") == bytes(expected_frame_raw)


def test_encode_string_data() -> None:
    string_data = "Hello"
    expected_frame_raw = (
        [START_BYTE, 0x15, len(string_data)]
        + [ord(x) for x in string_data]
        + [0x00, STOP_BYTE]
    )
    expected_frame_raw[-2] = compute_checksum(bytes(expected_frame_raw))
    assert encode(0x15, bytes(string_data, encoding="utf-8")) == bytes(
        expected_frame_raw,
    )


def test_encode_invalid_req_id() -> None:
    with pytest.raises(ValueError, match="bytes must be in "):
        encode(0x100, b"")


def test_decode_single_byte() -> None:
    raw_frame = [START_BYTE, 0x00, 0x01, 0xBB, 0x00, STOP_BYTE]
    raw_frame[-2] = compute_checksum(bytes(raw_frame))

    assert decode(bytes(raw_frame)) == (0x00, bytes([0xBB]))


def test_decode_multiple_bytes() -> None:
    random_data = list(random.randbytes(0x10))  # noqa: S311
    raw_frame = [START_BYTE, 0xCA, 0x10, *random_data, 0x00, STOP_BYTE]
    raw_frame[-2] = compute_checksum(bytes(raw_frame))

    assert decode(bytes(raw_frame)) == (0xCA, bytes(random_data))


def test_decode_empty_data() -> None:
    raw_frame = [START_BYTE, 0xFE, 0x00, 0x00, STOP_BYTE]
    raw_frame[-2] = compute_checksum(bytes(raw_frame))

    assert decode(bytes(raw_frame)) == (0xFE, b"")


def test_decode_invalid_start() -> None:
    raw_frame = [0x24, 0xFE, 0x01, 0x00, 0x00, STOP_BYTE]
    raw_frame[-2] = compute_checksum(bytes(raw_frame))

    with pytest.raises(ValueError, match="Invalid start byte"):
        decode(raw_frame)


def test_decode_invalid_stop() -> None:
    raw_frame = [START_BYTE, 0xFE, 0x01, 0x00, 0x00, 0x00]
    raw_frame[-2] = compute_checksum(bytes(raw_frame))

    with pytest.raises(ValueError, match="Invalid stop byte"):
        decode(raw_frame)


def test_decode_invalid_checksum() -> None:
    raw_frame = [START_BYTE, 0xFE, 0x01, 0x00, 0xCD, STOP_BYTE]

    with pytest.raises(ValueError, match="Invalid checksum byte"):
        decode(raw_frame)

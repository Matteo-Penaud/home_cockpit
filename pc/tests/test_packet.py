from protocol.packet import compute_checksum
import pytest

def test_compute_checksum_single_byte():
    assert compute_checksum(bytes([0x11])) == 0x11
    
def test_compute_checksum_multiple_bytes():
    data = bytes(range(10))
    result = 0
    for b in data:
        result ^= b
        
    assert compute_checksum(data) == result
    
def test_compute_checksum_empty():
    assert compute_checksum(b'') == 0
    
def test_compute_checksum_invalid_value():
    with pytest.raises(AssertionError):
        compute_checksum("Bonjour")
    
    with pytest.raises(AssertionError):
        compute_checksum(85)
def compute_checksum(data: bytes) -> int:
    try:
        result = 0
        for b in data:
            result ^= b
    except TypeError:
        raise AssertionError("Input data should be bytes")
    
    return result
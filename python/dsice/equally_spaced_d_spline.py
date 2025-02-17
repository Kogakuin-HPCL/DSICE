import os
import sys
import ctypes

class EquallySpacedDSpline:
    def __init__(self, sample_num:int, alpha:float=0.1, interpolated_point_num:int=2):

        if sys.platform.startswith("win"):
            shared_library_name = 'DSICE.dll'
        else:
            shared_library_name = 'libDSICE.so'

        # C 関数の情報を入力
        dll_path = os.path.join(os.path.dirname(__file__), shared_library_name)
        self.call_lib = ctypes.CDLL(dll_path)
        self.call_lib.DSPLINE_EQ_CREATE.argtypes = [ctypes.c_size_t, ctypes.c_double, ctypes.c_size_t]
        self.call_lib.DSPLINE_EQ_CREATE.restype = ctypes.c_size_t
        self.call_lib.DSPLINE_DELETE.argtypes = [ctypes.c_size_t]
        self.call_lib.DSPLINE_DELETE.restype = None
        self.call_lib.DSPLINE_UPDATE.argtypes = [ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double]
        self.call_lib.DSPLINE_UPDATE.restype = None
        self.call_lib.DSPLINE_UPDATE_MANY.argtypes = [ctypes.c_size_t, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_double)]
        self.call_lib.DSPLINE_UPDATE_MANY.restype = None
        self.call_lib.DSPLINE_GET_MARKER_VALUE.argtypes = [ctypes.c_size_t, ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_MARKER_VALUE.restype = ctypes.c_double
        self.call_lib.DSPLINE_GET_SAMPLE_VALUE.argtypes = [ctypes.c_size_t, ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_SAMPLE_VALUE.restype = ctypes.c_double
        self.call_lib.DSPLINE_GET_MARKER_VALUES.argtypes = [ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_MARKER_VALUES.restype = ctypes.POINTER(ctypes.c_double)
        self.call_lib.DSPLINE_GET_SAMPLE_VALUES.argtypes = [ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_SAMPLE_VALUES.restype = ctypes.POINTER(ctypes.c_double)
        self.call_lib.DSPLINE_MARKER_IDX_2_SAMPLE_IDX.argtypes = [ctypes.c_size_t, ctypes.c_size_t]
        self.call_lib.DSPLINE_MARKER_IDX_2_SAMPLE_IDX.restype = ctypes.c_size_t
        self.call_lib.DSPLINE_SAMPLE_IDX_2_MARKER_IDX.argtypes = [ctypes.c_size_t, ctypes.c_size_t]
        self.call_lib.DSPLINE_SAMPLE_IDX_2_MARKER_IDX.restype = ctypes.c_size_t
        self.call_lib.DSPLINE_GET_MARKERS_NUM.argtypes = [ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_MARKERS_NUM.restype = ctypes.c_size_t
        self.call_lib.DSPLINE_GET_SAMPLES_NUM.argtypes = [ctypes.c_size_t]
        self.call_lib.DSPLINE_GET_SAMPLES_NUM.restype = ctypes.c_size_t

        self.id = self.call_lib.DSPLINE_EQ_CREATE(sample_num, alpha, interpolated_point_num)

    def __del__(self):
        self.call_lib.DSPLINE_DELETE(self.id)

    def update_one(self, sample_idx:int, sample_value:float) -> None:
        self.call_lib.DSPLINE_UPDATE(self.id, sample_idx, sample_value)

    def update_many(self, sample_indexes:list, sample_values:list) -> None:
        self.call_lib.DSPLINE_UPDATE_MANY(self.id, len(sample_indexes), sample_indexes, sample_values)

    def get_marker_value(self, marker_idx:int) -> float:
        return self.call_lib.DSPLINE_GET_MARKER_VALUE(self.id, marker_idx)

    def get_sample_value(self, sample_idx:int) -> float:
        return self.call_lib.DSPLINE_GET_SAMPLE_VALUE(self.id, sample_idx)

    def get_marker_values(self) -> list:

        tmp_values = self.call_lib.DSPLINE_GET_MARKER_VALUES(self.id)
        tmp_num = self.call_lib.DSPLINE_GET_MARKERS_NUM(self.id)

        self._marker_values = list()
        for i in range(tmp_num):
            self._marker_values.append(float(tmp_values[i]))

        return self._marker_values

    def get_sample_values(self) -> list:

        tmp_values = self.call_lib.DSPLINE_GET_SAMPLE_VALUES(self.id)
        tmp_num = self.call_lib.DSPLINE_GET_SAMPLES_NUM(self.id)

        self._sample_values = list()
        for i in range(tmp_num):
            self._sample_values.append(float(tmp_values[i]))

        return self._sample_values

    def marker_idx2sample_idx(self, marker_idx:int) -> int:
        return self.call_lib.DSPLINE_MARKER_IDX_2_SAMPLE_IDX(self.id, marker_idx)

    def sample_idx2marker_idx(self, sample_idx:int) -> int:
        return self.call_lib.DSPLINE_SAMPLE_IDX_2_MARKER_IDX(self.id, sample_idx)

    def get_marker_num(self) -> int:
        return self.call_lib.DSPLINE_GET_MARKERS_NUM(self.id)

    def get_sample_num(self) -> int:
        return self.call_lib.DSPLINE_GET_SAMPLES_NUM(self.id)

import os
import sys
import ctypes

class _DsiceViewerBackground:
    def __init__(self):

        if sys.platform.startswith("win"):
            shared_library_name = 'DSICE.dll'
        else:
            shared_library_name = 'libDSICE.so'

        # C 関数の情報を入力
        dll_path = os.path.join(os.path.dirname(__file__), shared_library_name)
        self.call_lib = ctypes.CDLL(dll_path)
        self.call_lib.DSICE_VIEWER_GET_LINE_POINTS.argtypes = [ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t)]
        self.call_lib.DSICE_VIEWER_GET_LINE_POINTS.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_size_t))
        self.call_lib.DSICE_VIEWER_GET_LINE_POINTS_NUM.argtypes = [ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_size_t)]
        self.call_lib.DSICE_VIEWER_GET_LINE_POINTS_NUM.restype = ctypes.c_size_t

    def get_line_points(self, space:list, coordinate1:list, coordinate2:list) -> list:

        array_type = ctypes.c_size_t * len(space)
        c_space = array_type(*space)
        c_coordinate1 = array_type(*coordinate1)
        c_coordinate2 = array_type(*coordinate2)

        c_points = self.call_lib.DSICE_VIEWER_GET_LINE_POINTS(len(space), c_space, c_coordinate1, c_coordinate2)

        points = list()
        for i in range(self.call_lib.DSICE_VIEWER_GET_LINE_POINTS_NUM(len(space), c_space, c_coordinate1, c_coordinate2)):
            tmp_coordinate = list()
            for j in range(len(space)):
                tmp_coordinate.append(int(c_points[i][j]))
            points.append(tmp_coordinate)

        return points

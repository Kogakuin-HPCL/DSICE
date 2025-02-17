import os
import sys
import ctypes
from enum import Enum

class DSICE_EXECUTION_MODE(Enum):
    RECOMMENDED = 0,
    S_IPPE = 1,
    S_2017 = 2,
    S_2018 = 3,
    P_2024 = 4

class DSICE_DATABASE_METRIC_TYPE(Enum):
    Overwritten = 0,
    Average = 1

class Tuner:
    def __init__(self):
        
        if sys.platform.startswith("win"):
            shared_library_name = 'DSICE.dll'
        else:
            shared_library_name = 'libDSICE.so'

        # C 関数の情報を入力
        dll_path = os.path.join(os.path.dirname(__file__), shared_library_name)
        self.call_lib = ctypes.CDLL(dll_path)
        self.call_lib.DSICE_CREATE.argtypes = None
        self.call_lib.DSICE_CREATE.restype = ctypes.c_size_t
        self.call_lib.DSICE_DELETE.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_DELETE.restype = None
        self.call_lib.DSICE_APPEND_PARAMETER.argtypes = [ctypes.c_size_t, ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
        self.call_lib.DSICE_APPEND_PARAMETER.restype = None
        self.call_lib.DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE.argtypes = [ctypes.c_size_t, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.call_lib.DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE.restype = None
        self.call_lib.DSICE_SELECT_MODE.argtypes = [ctypes.c_size_t, ctypes.c_int]
        self.call_lib.DSICE_SELECT_MODE.restype = None
        self.call_lib.DSICE_CONFIG_DO_INITIAL_SEARCH.argtypes = [ctypes.c_size_t, ctypes.c_bool]
        self.call_lib.DSICE_CONFIG_DO_INITIAL_SEARCH.restype = None
        self.call_lib.DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER.argtypes = [ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
        self.call_lib.DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER.restype = None
        self.call_lib.DSICE_CONFIG_RECORD_LOG.argtypes = [ctypes.c_size_t, ctypes.c_bool]
        self.call_lib.DSICE_CONFIG_RECORD_LOG.restype = None
        self.call_lib.DSICE_CONFIG_LOWER_VALUE_IS_BETTER.argtypes = [ctypes.c_size_t, ctypes.c_bool]
        self.call_lib.DSICE_CONFIG_LOWER_VALUE_IS_BETTER.restype = None
        self.call_lib.DSICE_CONFIG_HIGHER_VALUE_IS_BETTER.argtypes = [ctypes.c_size_t, ctypes.c_bool]
        self.call_lib.DSICE_CONFIG_HIGHER_VALUE_IS_BETTER.restype = None
        self.call_lib.DSICE_CONFIG_SET_PARALLEL_MODE.argtypes = [ctypes.c_size_t, ctypes.c_bool]
        self.call_lib.DSICE_CONFIG_SET_PARALLEL_MODE.restype = None
        self.call_lib.DSICE_CONFIG_SET_DSPLINE_ALPHA.argtypes = [ctypes.c_size_t, ctypes.c_double]
        self.call_lib.DSICE_CONFIG_SET_DSPLINE_ALPHA.restype = None
        self.call_lib.DSICE_CONFIG_SET_DATABASE_METRIC_TYPE.argtypes = [ctypes.c_size_t, ctypes.c_int]
        self.call_lib.DSICE_CONFIG_SET_DATABASE_METRIC_TYPE.restype = None
        self.call_lib.DSICE_BEGIN.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_BEGIN.restype = ctypes.POINTER(ctypes.c_double)
        self.call_lib.DSICE_BEGIN_PARALLEL.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_BEGIN_PARALLEL.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
        self.call_lib.DSICE_GET_SUGGESTED_NUM.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_GET_SUGGESTED_NUM.restype = ctypes.c_size_t
        self.call_lib.DSICE_END.argtypes = [ctypes.c_size_t, ctypes.c_double]
        self.call_lib.DSICE_END.restype = None
        self.call_lib.DSICE_GET_PARAM_NUM.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_GET_PARAM_NUM.restype = ctypes.c_size_t
        self.call_lib.DSICE_GET_PARAM_LENGTH.argtypes = [ctypes.c_size_t, ctypes.c_size_t]
        self.call_lib.DSICE_GET_PARAM_LENGTH.restype = ctypes.c_size_t
        self.call_lib.DSICE_GET_TARGET_PARAMETERS.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_GET_TARGET_PARAMETERS.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
        self.call_lib.DSICE_GET_TENTATIVE_BEST_PARAMETER.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_GET_TENTATIVE_BEST_PARAMETER.restype = ctypes.POINTER(ctypes.c_double)
        self.call_lib.DSICE_IS_SEARCH_FINISHED.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_IS_SEARCH_FINISHED.restype = ctypes.c_bool
        self.call_lib.DSICE_END_TIME.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_END_TIME.restype = None
        self.call_lib.DSICE_END_PARALLEL.argtypes = [ctypes.c_size_t, ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
        self.call_lib.DSICE_END_PARALLEL.restype = None
        self.call_lib.DSICE_PRINT_TUNING_RESULT_STD.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_PRINT_TUNING_RESULT_STD.restype = None
        self.call_lib.DSICE_PRINT_TUNING_RESULT_FILE.argtypes = [ctypes.c_size_t, ctypes.c_char_p]
        self.call_lib.DSICE_PRINT_TUNING_RESULT_FILE.restype = None
        self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_STD.argtypes = [ctypes.c_size_t]
        self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_STD.restype = None
        self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_FILE.argtypes = [ctypes.c_size_t, ctypes.c_char_p]
        self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_FILE.restype = None
        self.call_lib.DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER.argtypes = [ctypes.c_size_t, ctypes.c_char_p, ctypes.c_double]
        self.call_lib.DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER.restype = None
        self.call_lib.DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER.argtypes = [ctypes.c_size_t, ctypes.c_double]
        self.call_lib.DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER.restype = None

        self.id = self.call_lib.DSICE_CREATE()

    def __del__(self):
        self.call_lib.DSICE_DELETE(self.id)

    def append_parameter(self, param:list) -> None:
        array_type = ctypes.c_double * len(param)
        c_param = array_type(*param)
        self.call_lib.DSICE_APPEND_PARAMETER(self.id, len(param), c_param)

    def append_parameter_by_linear_space(self, min:float, max:float, space_length:float) -> None:
        self.call_lib.DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE(self.id, min, max, space_length)

    def select_mode(self, mode:DSICE_EXECUTION_MODE) -> None:
        self.call_lib.DSICE_SELECT_MODE(self.id, mode)

    def config_do_initial_search(self, do_init_search:bool=True) -> None:
        self.call_lib.DSICE_CONFIG_DO_INITIAL_SEARCH(self.id, do_init_search)

    def config_specify_initial_parameter(self, initial_parameter:list) -> None:
        array_type = ctypes.c_double * len(initial_parameter)
        c_param = array_type(*initial_parameter)
        self.call_lib.DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER(self.id, c_param)

    def config_record_log(self, record_log:bool=True) -> None:
        self.call_lib.DSICE_CONFIG_RECORD_LOG(self.id, record_log)

    def config_lower_value_is_better(self, lower_value_is_better:bool=True) -> None:
        self.call_lib.DSICE_CONFIG_LOWER_VALUE_IS_BETTER(self.id, lower_value_is_better)

    def config_higher_value_is_better(self, higher_value_is_better:bool=True) -> None:
        self.call_lib.DSICE_CONFIG_HIGHER_VALUE_IS_BETTER(self.id, higher_value_is_better)

    def config_set_parallel_mode(self, parallel_mode:bool=True) -> None:
        self.call_lib.DSICE_CONFIG_SET_PARALLEL_MODE(self.id, parallel_mode)

    def config_set_d_spline_alpha(self, alpha:float) -> None:
        self.call_lib.DSICE_CONFIG_SET_DSPLINE_ALPHA(self.id, alpha)

    def config_set_database_metric_type(self, metric_type:DSICE_DATABASE_METRIC_TYPE) -> None:
        self.call_lib.DSICE_CONFIG_SET_DATABASE_METRIC_TYPE(self.id, metric_type)

    def get_suggested_next(self) -> list:

        param = list()
        for i in range(self.call_lib.DSICE_GET_PARAM_NUM(self.id)):
            param.append(float(self.call_lib.DSICE_BEGIN(self.id)[i]))

        return param

    def get_suggested_list(self) -> list:

        params = list()
        for i in range(self.call_lib.DSICE_GET_SUGGESTED_NUM(self.id)):
            params.append(list())
            for j in range(self.call_lib.DSICE_GET_PARAM_NUM(self.id)):
                params[i].append(float(self.call_lib.DSICE_BEGIN_PARALLEL(self.id)[i][j]))

        return params

    def set_metric_value(self, metric_value:float) -> None:
        self.call_lib.DSICE_END(self.id, metric_value)

    def set_time_performance(self) -> None:
        self.call_lib.DSICE_END_TIME(self.id)

    def set_metric_values_list(self, metric_values_list:list) -> None:
        array_type = ctypes.c_double * len(metric_values_list)
        c_values = array_type(*metric_values_list)
        self.call_lib.DSICE_END_PARALLEL(self.id, len(metric_values_list), c_values)

    def get_param_num(self) -> int:
        return self.call_lib.DSICE_GET_PARAM_NUM(self.id)

    def get_param_length(self, parameter_idx:int) -> int:
        return self.call_lib.DSICE_GET_PARAM_LENGTH(self.id, parameter_idx)

    def get_target_parameters(self) -> list:

        target_parameters = list()
        for i in range(self.call_lib.DSICE_GET_PARAM_NUM(self.id)):
            tmp = list()
            for j in range(self.call_lib.DSICE_GET_PARAM_LENGTH(self.id, i)):
                tmp.append(self.call_lib.DSICE_GET_TARGET_PARAMETERS(self.id)[i][j])
            target_parameters.append(tmp)

        return target_parameters

    def get_tentative_best_parameter(self) -> list:

        param = list()
        for i in range(self.call_lib.DSICE_GET_PARAM_NUM(self.id)):
            param.append(float(self.call_lib.DSICE_GET_TENTATIVE_BEST_PARAMETER(self.id)[i]))

        return param

    def is_search_finished(self) -> bool:
        return self.call_lib.DSICE_IS_SEARCH_FINISHED(self.id)

    def print_tuning_result(self, file_path:str=None) -> None:
        if file_path is None:
            self.call_lib.DSICE_PRINT_TUNING_RESULT_STD(self.id)
        else:
            self.call_lib.DSICE_PRINT_TUNING_RESULT_FILE(self.id, file_path)

    def print_simple_loop_log(self, file_path:str=None) -> None:
        if file_path is None:
            self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_STD(self.id)
        else:
            self.call_lib.DSICE_PRINT_SIMPLE_LOOP_LOG_FILE(self.id, file_path)

    def output_search_space_for_viewer(self, file_path:str='search_space.log', default_value:float=0) -> None:
        self.call_lib.DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER(self.id, file_path, default_value)

    def output_log_for_viewer(self, file_path:str='dsice_sampling.log') -> None:
        self.call_lib.DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER(self.id, file_path)

import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from tkinter import messagebox
from ._dsice_viewer_background import _DsiceViewerBackground
from ._dsice_viewer_axis_config_window import _AxisConfigWindow
from ._dsice_viewer_show_suggested_window import _ShowSuggestedWindow
from ._dsice_viewer_dsp_entry_window import _DspEntryWindow
from ._dsice_viewer_show_dsp_window import _DspWindow
from ._dsice_viewer_show_measured_window import _ShowMeasuredWindow
from ._dsice_viewer_version_window import _VersionWindow

class Viewer(tk.Tk):
    def __init__(self):
        super().__init__()

        # 基本設定項目
        self.__basic_font = ('Helvetica', 14)
        self.__window_size = '1100x500'
        self.__window_bg_color = 'white'
        self.__highlight_color = 'orange'
        self.__color_bar_type = 'copper'
        self.__heat_map_origin = 'lower'
        self.__marker_size_rate = 0.7
        self.__annotate_font_rate = 0.3

        # ウィンドウ設定
        self.title('DSICE Viewer')
        self.geometry(self.__window_size)
        self.configure(bg=self.__window_bg_color)
        self.bind('<Destroy>', self.__closed_window_event)
        self.bind('<Configure>', self.__window_size_changed_event)

        # メンバ変数の初期化
        self.background = _DsiceViewerBackground()
        self.__plotted_base_coordinate_log = list()   # [coordinate]
        self.__total_loop_num = 0
        self.__now_loop = 0
        self.__plot_coordinates = set()   # {(x, y)}
        self.__plot_objs = list() # [matplotlib.lines.Line2D]
        self.__annotate_objs = list() # [matplotlib.text.Annotation]
        self.__last_width = self.winfo_width()
        self.__last_height = self.winfo_height()
        self.__parameter_lens = None  # [int]
        self.__parameter_usage = None # [int]
        self.__parameter_start = None # [int]
        self.__parameter_axis_step = None # [int]
        self.__dsice_base_log = None          # [coordinate]
        self.__dsice_suggest_log = None       # [[coordinate]]
        self.__dsice_measured_log = None      # [[coordinate]]
        self.__suggest_window = None
        self.__dsp_windows = list()
        self.__measured_window = None
        self.__space_values = None  # {tuple(coordinate):float}

        # メニューバー作成
        self.__create_menubar()

        # 描画枠作成
        self.__create_canvas()

        # データパネル作成
        self.__create_data_panel()

        # ステータスバー作成
        self.__create_statusbar()

        # ウィジェット配置調整
        self.columnconfigure(0, weight=6)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1)
        self.columnconfigure(3, weight=1)
        self.columnconfigure(4, weight=1)
        self.rowconfigure(99, weight=1)  # ステータスバーを除く最終行 (canvas のみの行) を伸ばす

    #
    # イベントメソッド
    #

    def __closed_window_event(self, event) -> None:
        """
        ウィンドウが閉じたときに呼び出されるイベントメソッド
        """
        plt.close()     # fig が tkinter と別に独自ループを持つらしく，閉じる前に close() が必要

    def __window_size_changed_event(self, event) ->None:
        """
        ウィンドウサイズが変更されたときに呼び出され，内部要素のサイズ調整を行うメソッド
        """
        if event.width != self.__last_width or event.height != self.__last_height:
            for plot_obj in self.__plot_objs:
                plot_obj.set_markersize(self.__calc_plot_marker_size())
            for text_obj in self.__annotate_objs:
                text_obj.set_fontsize(self.__calc_annotate_font_size())
            self.__last_width = event.width
            self.__last_height = event.height

    #
    # GUI 要素作成
    #

    def __create_menubar(self) -> None:
        """
        上部メニューバーの作成
        """
        self.__menubar = _ViewerMenu(self)
        self.config(menu=self.__menubar)
        self.__menubar._comm_load_search_space = self.__load_search_space
        self.__menubar._comm_load_dsice_log = self.__load_dsice_log
        self.__menubar._comm_open_axis_config_window = self.__open_axis_config_window
        self.__menubar._comm_open_suggest_window = self.__open_suggest_window
        self.__menubar._comm_open_dsp_window = self.__open_dsp_entry
        self.__menubar._comm_open_measured_window = self.__open_measured_window
        self.__menubar._comm_open_version_window = self.__open_version_window

    def __create_canvas(self) -> None:
        """
        メインキャンバスを作成
        """
        self.__main_fig = plt.figure()
        self.__main_canvas = FigureCanvasTkAgg(self.__main_fig, master=self)
        self.__main_canvas.draw()
        self.__main_canvas.get_tk_widget().grid(row=0, column=0, rowspan=100, sticky=tk.NSEW)

    def __create_data_panel(self) -> None:
        """
        データパネルを作成
        """

        # ダミー
        dummy_label1 = tk.Label(self, text='', bg=self.__window_bg_color)
        dummy_label1.grid(row=0, column=1)

        # ループ数表示
        self.__loop_num_label = tk.Label(self, text='Loop :', font=self.__basic_font, bg=self.__window_bg_color, justify='left')
        self.__loop_num_label.config(bg='white')
        self.__loop_num_label.grid(row=1, column=1, columnspan=4, sticky=tk.W)

        # ループ数変化
        self.__loop_num_change_pre_bt = tk.Button(self, text='<', font=self.__basic_font, state=tk.DISABLED, justify='left', command=self.__draw_dsice_log_previous)
        self.__loop_num_change_pre_bt.grid(row=2, column=1)
        self.__loop_num_change_next_bt = tk.Button(self, text='>', font=self.__basic_font, state=tk.DISABLED, justify='left', command=self.__draw_dsice_log_next)
        self.__loop_num_change_next_bt.grid(row=2, column=3)
        
        # ダミー
        dummy_label2 = tk.Label(self, text='', bg=self.__window_bg_color)
        dummy_label2.grid(row=3, column=1)

        # 基準点表示
        self.__base_point_label = tk.Label(self, text='Base Point :', font=self.__basic_font, bg=self.__window_bg_color, justify='left')
        self.__base_point_label.config(bg=self.__window_bg_color)
        self.__base_point_label.grid(row=4, column=1, columnspan=4, sticky=tk.W)

        # ダミー
        dummy_label3 = tk.Label(self, text='', bg=self.__window_bg_color)
        dummy_label3.grid(row=5, column=1)

        # 軸設定タイトル
        self.__axis_setting_title_label = tk.Label(self, text='Parameters', font=self.__basic_font, bg=self.__window_bg_color, justify='left')
        self.__axis_setting_title_label.grid(row=6, column=1, columnspan=4, sticky=tk.W)

        # 軸設定情報
        self.__axis_setting_value_label = tk.Label(self, text='', font=self.__basic_font, bg=self.__window_bg_color, justify='left')
        self.__axis_setting_value_label.grid(row=7, column=1, columnspan=4, sticky=tk.W)

    def __create_statusbar(self) ->None:
        """
        ステータスバーを作成
        """
        self.__statusbar = tk.Label(self, text="準備完了 (ファイルロード待ち)", bd=1, relief=tk.SUNKEN, anchor=tk.W, bg='#e6e6fa', font=('Helvetica', 10))
        self.__statusbar.grid(row=100, column=0, columnspan=5, sticky=tk.EW)

    #
    # ユーザ操作コマンド
    #

    def __open_axis_config_window(self) -> None:
        """
        軸設定ウィンドウを起動
        """

        acw = _AxisConfigWindow(self, len(self.__parameter_lens), self.__basic_font, self.__window_bg_color, self.__parameter_usage, self.__parameter_start, self.__parameter_axis_step)
        acw.grab_set()
        acw.wait_window()

    def __open_suggest_window(self) -> None:
        """
        DSICE により実測提案された点をプロットするウィンドウを起動
        """

        # ウィンドウがすでに存在していればアクティブ化だけする
        if self.__suggest_window != None and self.__suggest_window.winfo_exists():
            self.__suggest_window.focus_set()
            return

        self.__statusbar.config(text='候補点ウィンドウを起動中……')
        self.__suggest_window = _ShowSuggestedWindow(font=self.__basic_font, bg=self.__window_bg_color, color_bar_type=self.__color_bar_type, marker_size_rate=self.__marker_size_rate, annotate_font_rate=self.__annotate_font_rate)
        self.__suggest_window._set_heat_map(self.__now_heat_map, self.__heatmap_coordinates)
        self.__suggest_window._update_heat_map()
        if self.__now_loop != 0:
            self.__suggest_window._plot_suggested(self.__dsice_suggest_log[self.__now_loop - 1])
        self.__suggest_window.focus_set()
        self.__statusbar.config(text='候補点ウィンドウを起動しました')

    def __open_dsp_entry(self) -> None:
        """
        どの直線の d-Spline 近似状態を描画するかを指定するウィンドウを表示
        """

        dspe = _DspEntryWindow(self, self.__basic_font, self.__window_bg_color, self.__parameter_lens)
        dspe.grab_set()
        dspe.wait_window()

    def __open_measured_window(self) -> None:
        """
        DSICE により実測された点の座標を列挙するウィンドウを表示
        """

        # ウィンドウがすでに存在していればアクティブ化だけする
        if self.__measured_window != None and self.__measured_window.winfo_exists():
            self.__measured_window.focus_set()
            return

        self.__statusbar.config(text='実測座標表示ウィンドウを起動中……')
        self.__measured_window = _ShowMeasuredWindow(self.__basic_font, self.__window_bg_color)
        if self.__now_loop != 0:
            self.__measured_window._view_measured(self.__dsice_measured_log[self.__now_loop - 1])
        self.__measured_window.focus_set()
        self.__statusbar.config(text='実測座標表示ウィンドウを起動しました')

    def __open_version_window(self) -> None:
        """
        バージョンウィンドウを表示
        """

        version_window = _VersionWindow(font=self.__basic_font, bg=self.__window_bg_color)
        version_window.grab_set()
        version_window.wait_window()

    #
    # 初期状態生成
    #

    def __set_initial_axis_config(self) -> None:
        """
        メインキャンバスの軸 (どのパラメタがどの軸に入るか) を初期化
        """

        self.__parameter_usage = list()
        for i in range(len(self.__parameter_lens)):
            if i == 0:
                self.__parameter_usage.append(0)
            elif i == 1:
                self.__parameter_usage.append(1)
            else:
                self.__parameter_usage.append(2)

        self.__parameter_start = list()
        for i in range(len(self.__parameter_lens)):
            self.__parameter_start.append(0)
        self.__parameter_axis_step = list()
        for i in range(len(self.__parameter_lens)):
            self.__parameter_axis_step.append(1)

    def __unload_dsice_log(self) -> None:
        """
        ロード済 DSICE ログデータを削除
        """

        self.__plotted_base_coordinate_log = list()
        self.__total_loop_num = 0
        self.__now_loop = 0
        self.__plot_coordinates = set()
        self.__plot_objs = list()
        self.__annotate_objs = list()
        self.__dsice_base_log = None
        self.__dsice_suggest_log = None
        self.__dsice_measured_log = None
        self.__update_loop_count_label()
        self.__update_loop_button()
        self.__update_base_show_label()

    #
    # 設定更新系処理
    #

    def _config_axes(self, usage:list, start:list, step:list) -> None:
        """
        軸設定ウィンドウから呼び出し，軸設定を実行

        Args:
            usage (list): 各パラメタがどの軸に属するか
            start (list): 軸[0] に対応する各パラメタのインデックス
            step (list): 軸インデックスが 1 進むごとに各パラメタがいくつ進むか

        Raises:
            ValueError: 各引数が全パラメタの情報を持たない場合
        """

        if len(usage) != len(self.__parameter_lens) or len(start) != len(self.__parameter_lens) or len(step) != len(self.__parameter_lens):
            raise ValueError('The length of each argument must be equal to the number of parameters.')

        self.__parameter_usage = usage
        self.__parameter_start = start
        self.__parameter_axis_step = step

        self.__statusbar.config(text='軸情報更新中……')
        self.__create_axes()
        self.__statusbar.config(text='ヒートマップ更新中……')
        self.__update_heatmap(create_new_map=True)
        if self.__total_loop_num != 0:
            self.__draw_dsice_log_specific_loop(self.__now_loop)
        self.__statusbar.config(text='データパネル更新中……')
        self.__update_data_panel()
        self.__statusbar.config(text='軸ラベル更新中……')
        self.__update_axis_label()
        self.__sync_sub_windows(sync_heatmap=True)
        self.__statusbar.config(text='軸を設定しました')

    def _open_dsp_window(self, coordinate1:list, coordinate2:list) -> None:
        """
        d-Spline による近似状態を描画するウィンドウを表示

        Args:
            coordinate1 (list): 表示する直線上の 1 点目
            coordinate2 (list): 表示する直線上の 2 点目
        """

        line_coordinates = self.background.get_line_points(self.__parameter_lens, coordinate1, coordinate2)

        self.__dsp_windows.append(_DspWindow(self, self.__window_bg_color, self.__space_values, line_coordinates, self.__dsice_measured_log, self.__now_loop))
        self.__dsp_windows[-1].focus_set()

    #
    # サブウィンドウ同期系処理
    #

    def __sync_sub_windows(self, sync_heatmap=False) -> None:
        """
        表示ループ更新時にサブウィンドウと同期をとる

        Args:
            sync_heatmap (bool): ヒートマップの更新が必要な場合に True. Defaults to False.
        """

        self.__sync_suggest_window(sync_heatmap)
        self.__sync_dsp_window()
        self.__sync_measured_window()

    def __sync_suggest_window(self, sync_heatmap=False) -> None:
        """
        候補点ウィンドウと同期をとる

        Args:
            sync_heatmap (bool): ヒートマップの更新が必要な場合に True. Defaults to False.
        """

        if self.__suggest_window != None and self.__suggest_window.winfo_exists():
            self.__statusbar.config(text='候補点ウィンドウ同期中……')
            if sync_heatmap:
                self.__suggest_window._set_heat_map(self.__now_heat_map, self.__heatmap_coordinates)
                self.__suggest_window._update_heat_map()
            if self.__now_loop != 0:
                self.__suggest_window._plot_suggested(self.__dsice_suggest_log[self.__now_loop - 1])
            else:
                self.__suggest_window._plot_suggested([])
            self.__statusbar.config(text='候補点ウィンドウの同期完了')

    def __sync_dsp_window(self) -> None:
        """
        d-Spline 近似表示ウィンドウと同期をとる
        """

        if self.__now_loop == 0:
            return

        self.__statusbar.config(text='d-Spline 近似ウィンドウ同期中……')

        for dspw in self.__dsp_windows:
            if dspw.winfo_exists():
                dspw._plot(self.__now_loop)

        self.__statusbar.config(text='d-Spline 近似ウィンドウの同期完了')

    def __sync_measured_window(self) -> None:
        """
        実測座標表示ウィンドウと同期をとる
        """

        if self.__measured_window != None and self.__measured_window.winfo_exists():
            self.__statusbar.config(text='実測座標表示ウィンドウ同期中……')
            if self.__now_loop != 0:
                self.__measured_window._view_measured(self.__dsice_measured_log[self.__now_loop - 1])
            else:
                self.__measured_window._view_measured([])
            self.__statusbar.config(text='実測座標表示ウィンドウの同期完了')

    #
    # データロードメソッド
    #

    def __load_search_space(self) -> None:
        """
        探索空間のロード処理を開始，実行
        """

        search_space_file_path = tk.filedialog.askopenfilename(title='探索空間のすべての値データを選択')
        if search_space_file_path == '':
            self.__statusbar.config(text='ファイル読み込みをキャンセルしました')
            return

        self.__statusbar.config(text='探索空間ファイルを読み込み中……')

        # データ取得
        try:
            self.__load_search_space_file(search_space_file_path)
        except:
            self.__statusbar.config(text='探索空間の読み込みを中断しました')
            return

        # DSICE ログを読み込み済みの場合は削除
        if self.__total_loop_num != 0:
            self.__statusbar.config(text='DSICE ログを削除中……')
            self.__unload_dsice_log()

        self.__statusbar.config(text='軸情報更新中……')
        self.__set_initial_axis_config()
        self.__create_axes()
        self.__statusbar.config(text='ヒートマップ更新中……')
        self.__update_heatmap(create_new_map=True)
        self.__statusbar.config(text='データパネル更新中……')
        self.__update_data_panel()
        self.__statusbar.config(text='軸ラベル更新中……')
        self.__update_axis_label()
        self.__sync_sub_windows(sync_heatmap=True)
        self.__menubar._change_state('DSICE ログファイル読み込み', state='normal')
        self.__menubar._change_state('描画軸設定', state='normal')
        self.__menubar._change_state('候補点ウィンドウ', state='normal')
        self.__menubar._change_state('d-Spline 近似ウィンドウ', state='normal')
        self.__menubar._change_state('実測座標表示ウィンドウ', state='normal')
        self.__statusbar.config(text='探索空間を読み込みました')

    def __load_search_space_file(self, file_path:str) -> None:
        """
        探索空間全点の値をファイルから読み込む

        Args:
            file_path (str): 読み込むファイルのパス
        """

        with open(file_path, 'r', encoding='UTF-8') as f:

            line = f.readline()
            while '#' in line:
                line = f.readline()
            if not line:
                messagebox.showerror('ロード失敗', '指定されたファイルには必要な情報がありません')
                raise Exception('load failed.')
            try:
                self.__parameter_lens = self.__load_parameter_str(line)
            except:
                messagebox.showerror('ロード失敗', '指定されたファイルには必要な情報がありません')
                raise Exception('load failed.')

            self.__space_values = dict()
            while True:

                line = f.readline()
                if not line:
                    break
                elif '#' in line:
                    continue

                tmp_line = line.split(' ')
                tmp_coordinate = list()
                for c in tmp_line[:-1]:
                    tmp_coordinate.append(int(c))
                self.__space_values[tuple(tmp_coordinate)] = float(tmp_line[-1:][0])

    def __load_dsice_log(self) -> None:
        """
        DSICE の探索ログデータロード処理を開始，実行
        """

        dsice_log_file_path = tk.filedialog.askopenfilename(title='DSICE のログを選択')
        if dsice_log_file_path == '':
            self.__statusbar.config(text='ファイル読み込みをキャンセルしました')
            return

        # データ取得
        self.__statusbar.config(text='DSICE のログを読み込み中……')
        try:
            self.__load_dsice_log_file(dsice_log_file_path)
        except:
            self.__statusbar.config(text='DSICE のログ読み込みを中断しました')
            return

        # プロット状態を更新
        self.__statusbar.config(text='DSICE のログをプロット中……')
        self.__draw_dsice_log_specific_loop(1)
        self.__statusbar.config(text='DSICE のログを読み込みました')

    def __load_dsice_log_file(self, file_path:str) -> list:
        """
        DSICE の動作ログファイルを読み込む

        Args:
            file_path (str): 読み込むファイルのパス
        """

        with open(file_path, 'r', encoding='UTF-8') as f:

            line = f.readline()
            while '#' in line:
                line = f.readline()
            if not line:
                messagebox.showerror('ロード失敗', '指定されたファイルには必要な情報がありません')
                raise Exception('load failed.')
            try:
                tmp_parameter_lens = self.__load_parameter_str(line)
            except:
                messagebox.showerror('ロード失敗', '指定されたファイルには必要な情報がありません')
                raise Exception('load failed.')
            if tmp_parameter_lens != self.__parameter_lens:
                messagebox.showerror('ロード失敗', '指定されたファイルの性能パラメタ情報は探索空間と異なります')
                raise Exception('load failed.')

            self.__dsice_base_log = list()
            self.__dsice_suggest_log = list()
            self.__dsice_measured_log = list()

            loop = 0
            is_bp = False   # base
            is_sp = False   # suggest
            is_mp = False   # measure
            while True:
                line = f.readline()
                if not line:
                    break
                elif '#' in line:
                    continue
                elif line == '':
                    continue
                line_elements = line.split()
                if line_elements[0] == 'loop':
                    if line_elements[2] == 'start':
                        loop = int(line_elements[1])
                        self.__dsice_suggest_log.append(list())
                        self.__dsice_measured_log.append(list())
                    else:
                        pass
                elif line_elements[0] == 'base':
                    if line_elements[1] == 'start':
                        is_bp = True
                    else:
                        is_bp = False
                elif line_elements[0] == 'suggested':
                    if line_elements[1] == 'start':
                        is_sp = True
                    else:
                        is_sp = False
                elif line_elements[0] == 'measured':
                    if line_elements[1] == 'start':
                        is_mp = True
                    else:
                        is_mp = False
                else:
                    tmp_coordinate = list()
                    for c in line_elements:
                        tmp_coordinate.append(int(c))
                    if is_bp:
                        self.__dsice_base_log.append(tmp_coordinate)
                    elif is_mp:
                        self.__dsice_measured_log[-1].append(tmp_coordinate)
                    elif is_sp:
                        self.__dsice_suggest_log[-1].append(tmp_coordinate)
        self.__total_loop_num = len(self.__dsice_base_log)

    #
    # データ加工・格納メソッド
    #

    def __create_axes(self) -> None:
        """
        設定された軸情報から各軸の本当の値を取得
        """

        # 各パラメタの座標を確認
        self.__axis_x_len = 0 # x 軸の要素数
        self.__axis_y_len = 0 # y 軸の要素数
        tmp_axis_x = list() # 複雑になるのでいったん転置で保持
        tmp_axis_y = list() # 複雑になるのでいったん転置で保持
        tmp_axis_constant = list()
        for i in range(len(self.__parameter_usage)):

            tmp_pos = self.__parameter_start[i]
            tmp_axis_x.append(list())
            tmp_axis_y.append(list())

            if self.__parameter_usage[i] == 0:
                # 横軸に使用
                tmp_axis_constant.append(-1)
                while 0 <= tmp_pos < self.__parameter_lens[i]:
                    tmp_axis_x[-1].append(tmp_pos)
                    tmp_pos += self.__parameter_axis_step[i]
                if len(tmp_axis_x[-1]) != 0:
                    if self.__axis_x_len == 0:
                        self.__axis_x_len = len(tmp_axis_x[-1])
                    else:
                        self.__axis_x_len = min([self.__axis_x_len, len(tmp_axis_x[-1])])
            elif self.__parameter_usage[i] == 1:
                # 縦軸に使用
                tmp_axis_constant.append(-1)
                while 0 <= tmp_pos < self.__parameter_lens[i]:
                    tmp_axis_y[-1].append(tmp_pos)
                    tmp_pos += self.__parameter_axis_step[i]
                if len(tmp_axis_y[-1]) != 0:
                    if self.__axis_y_len == 0:
                        self.__axis_y_len = len(tmp_axis_y[-1])
                    else:
                        self.__axis_y_len = min([self.__axis_y_len, len(tmp_axis_y[-1])])
            else:
                # 指定値を固定値にする
                if 0 <= tmp_pos < self.__parameter_lens[i]:
                    tmp_axis_constant.append(tmp_pos)

        # 軸を生成
        self.__axis_x = list()
        self.__axis_y = list()
        for i in range(self.__axis_x_len):
            self.__axis_x.append(list())
        for i in range(self.__axis_y_len):
            self.__axis_y.append(list())
        for i in range(len(self.__parameter_lens)):
            if self.__parameter_usage[i] == 0:
                for j in range(self.__axis_x_len):
                    self.__axis_x[j].append(tmp_axis_x[i][j])
                for j in range(self.__axis_y_len):
                    self.__axis_y[j].append(-1)
            elif self.__parameter_usage[i] == 1:
                for j in range(self.__axis_x_len):
                    self.__axis_x[j].append(-1)
                for j in range(self.__axis_y_len):
                    self.__axis_y[j].append(tmp_axis_y[i][j])
            else:
                for j in range(self.__axis_x_len):
                    self.__axis_x[j].append(tmp_axis_constant[i])
                for j in range(self.__axis_y_len):
                    self.__axis_y[j].append(tmp_axis_constant[i])

    #
    # 描画処理
    #

    def __update_heatmap(self, create_new_map:bool=False) -> None:
        """
        ヒートマップを再描画

        Args:
            create_new_map (bool): 新規でヒートマップを作成するか
        """

        if create_new_map:
            (self.__now_heat_map, self.__heatmap_coordinates) = self.__create_heat_map_data()

        x_tick = list()
        y_tick = list()

        if len(self.__now_heat_map[0]) < 11:
            for i in range(len(self.__now_heat_map[0])):
                x_tick.append(i)
        else:
            for i in range(11):
                x_tick.append(int(len(self.__now_heat_map[0]) - 1) * (i / 10))
        if len(self.__now_heat_map) < 11:
            for i in range(len(self.__now_heat_map)):
                y_tick.append(i)
        else:
            for i in range(11):
                y_tick.append(int(len(self.__now_heat_map) - 1) * (i / 10))

        self.__main_fig.clear()
        ax = self.__main_fig.add_subplot(111)
        im = ax.imshow(self.__now_heat_map, cmap=self.__color_bar_type, origin=self.__heat_map_origin)
        ax.set_xticks(x_tick)
        ax.set_yticks(y_tick)
        self.__main_fig.colorbar(im)
        self.__main_canvas.draw()

    def __plot_dsice_log(self) -> None:
        """
        self.__now_loop のループをプロットします．

        処理軽量化のため，この段階では canvas の draw は行いません
        """

        marker_size = self.__calc_plot_marker_size()
        tmp_loop = self.__now_loop - 1
        if len(self.__plotted_base_coordinate_log) == 0 or self.__dsice_base_log[tmp_loop] != self.__plotted_base_coordinate_log[-1]:
            self.__plotted_base_coordinate_log.append(self.__dsice_base_log[tmp_loop])
            tmp_base = tuple(self.__dsice_base_log[tmp_loop])
            if tmp_base in self.__heatmap_coordinates:
                x, y = self.__heatmap_coordinates[tmp_base]
                ax = self.__main_fig.get_axes()[0]
                plot_obj, = ax.plot(x, y, marker='o', markersize=marker_size, markerfacecolor='none', markeredgecolor='lime', markeredgewidth=3, linestyle='None')
                self.__plot_objs.append(plot_obj)
        for c in self.__dsice_measured_log[tmp_loop]:
            if tuple(c) in self.__heatmap_coordinates:
                x, y = self.__heatmap_coordinates[tuple(c)]
                if (x[0], y[0]) in self.__plot_coordinates:
                    continue
                ax = self.__main_fig.get_axes()[0]
                # 過去基準点になったことのある点ではエッジが lime
                if c in self.__plotted_base_coordinate_log:
                    plot_obj, = ax.plot(x, y, marker='o', markersize=marker_size, color='white', markeredgecolor='lime', markeredgewidth=3, linestyle='None')
                else:
                    plot_obj, = ax.plot(x, y, marker='o', markersize=marker_size, color='white', markeredgecolor='white', linestyle='None')
                self.__plot_coordinates.add((x[0], y[0]))
                self.__plot_objs.append(plot_obj)
                text_obj = ax.annotate(f'{self.__now_loop}', xy=(x[0], y[0]), ha='center', va='center', fontsize=self.__calc_annotate_font_size())
                self.__annotate_objs.append(text_obj)
        self.__sync_sub_windows()

    def __draw_dsice_log_next(self) -> None:
        """
        self.__now_loop を 1 進め，挙動データを描画します．
        """

        if self.__now_loop >= self.__total_loop_num:
            return

        self.__loop_front()
        self.__plot_dsice_log()
        self.__main_canvas.draw()
        self.__update_base_show_label()
        self.__statusbar.config(text='')

    def __draw_dsice_log_previous(self) -> None:
        """
        self.__now_loop を 1 戻し，そのループまでの挙動データを描画します．

        再描画となるため，描画完了までにやや時間がかかる場合があります
        """

        if self.__total_loop_num <= 1:
            return

        self.__draw_dsice_log_specific_loop(self.__now_loop - 1)
        self.__statusbar.config(text='')

    def __draw_dsice_log_specific_loop(self, loop:int) -> None:
        """
        self.__now_loop を引数 loop にし，そのループまでの挙動データを描画します．<br/>
        元の self.__now_loop より前のループを指定した場合は再描画となるため，描画完了までにやや時間がかかる場合があります

        Args:
            loop (int): 最終的に描画させるループ番号 (1 オリジン)
        """

        if loop < 1 or self.__total_loop_num < loop:
            return
        if loop > self.__now_loop:
            for i in range(loop - self.__now_loop):
                self.__loop_front()
                self.__plot_dsice_log()
        else:
            self.__plotted_base_coordinate_log = list()
            self.__plot_coordinates = set()
            self.__plot_objs = list()
            self.__update_heatmap()
            self.__now_loop = 0
            for i in range(loop):
                self.__loop_front()
                self.__plot_dsice_log()
        self.__now_loop = loop
        self.__loop_num_label.config(text=f'Loop : {self.__now_loop}')
        self.__update_axis_label()
        self.__update_loop_count_label()
        self.__update_loop_button()
        self.__update_base_show_label()
        self.__main_canvas.draw()

    def __loop_front(self) -> None:
        """
        ログのループ番号を 1 進めます．連動して，ボタンの有効 / 無効が変化します．
        """

        if self.__now_loop >= self.__total_loop_num:
            return

        self.__now_loop += 1
        self.__update_loop_count_label()
        self.__update_loop_button()

    def __loop_back(self) -> None:
        """
        ログのループ番号を 1 戻します．連動して，ボタンの有効 / 無効が変化します．
        """

        if self.__now_loop <= 1:
            return

        self.__now_loop -= 1
        self.__update_loop_count_label()
        self.__update_loop_button()

    def __update_loop_count_label(self) -> None:
        if self.__total_loop_num == 0:
            self.__loop_num_label.config(text='Loop :')
        else:
            self.__loop_num_label.config(text=f'Loop : {self.__now_loop}')

    def __update_loop_button(self) -> None:
        """
        self.__now_loop に応じて，ループ増減ボタンの有効 / 無効を切り替える
        """

        if self.__now_loop <= 1:
            self.__loop_num_change_pre_bt.config(state='disabled')
        else:
            self.__loop_num_change_pre_bt.config(state='normal')
        if self.__now_loop >= self.__total_loop_num:
            self.__loop_num_change_next_bt.config(state='disabled')
        else:
            self.__loop_num_change_next_bt.config(state='normal')

    def __update_base_show_label(self) -> None:
        """
        基準座標ラベルの表示を更新
        """

        if self.__total_loop_num == 0:
            self.__base_point_label.config(text='Base Point :')
        else:
            self.__base_point_label.config(text=f'Base Point : {self.__dsice_base_log[self.__now_loop - 1]}')
            if self.__now_loop != 1 and self.__dsice_base_log[self.__now_loop - 1] != self.__dsice_base_log[self.__now_loop - 2]:
                self.__base_point_label.config(bg=self.__highlight_color)
            else:
                self.__base_point_label.config(bg=self.__window_bg_color)

    def __update_data_panel(self) -> None:
        """
        データパネルの表示を更新
        """

        tmp_text = ''
        for i in range(len(self.__parameter_lens)):
            tmp_text += f'param{i + 1} : '
            if self.__parameter_usage[i] == 0:
                tmp_text += f'axis_x (start : {self.__parameter_start[i]}, step : {self.__parameter_axis_step[i]})\n'
            elif self.__parameter_usage[i] == 1:
                tmp_text += f'axis_y (start : {self.__parameter_start[i]}, step : {self.__parameter_axis_step[i]})\n'
            else:
                tmp_text += f'constant ({self.__axis_x[0][i]})\n'
        self.__axis_setting_value_label.config(text=tmp_text)

    def __update_axis_label(self) -> None:
        """
        軸ラベルの表示を更新
        """

        axis_x_param = list()
        axis_y_param = list()
        for i in range(len(self.__parameter_lens)):
            if self.__parameter_usage[i] == 0:
                axis_x_param.append(i)
            elif self.__parameter_usage[i] == 1:
                axis_y_param.append(i)
        axis_x_label_text = ''
        axis_y_label_text = ''
        if len(axis_x_param) != 0:
            axis_x_label_text = f'Axis X ({axis_x_param[0]}'
            for i in range(1, len(axis_x_param)):
                axis_x_label_text += f', {axis_x_param[i]}'
            axis_x_label_text += ')'
        if len(axis_y_param) != 0:
            axis_y_label_text = f'Axis Y ({axis_y_param[0]}'
            for i in range(1, len(axis_y_param)):
                axis_y_label_text += f', {axis_y_param[i]}'
            axis_y_label_text += ')'
        ax = self.__main_fig.get_axes()[0]
        ax.set_xlabel(axis_x_label_text)
        ax.set_ylabel(axis_y_label_text)
        self.__main_canvas.draw()

    #
    # 補助メソッド
    #

    def __calc_plot_marker_size(self) -> float:
        """
        ウィンドウサイズに応じたプロットマーカサイズを算出

        Returns:
            float: 算出されたマーカサイズ
        """

        POINTS_PER_INCH = 72 # DTP の基準ポイント (matplotlib の引数が DTP などの「ポイント」を使っているらしい)

        fig_width_inches, fig_height_inches = self.__main_fig.get_size_inches()
        fig_width_points = fig_width_inches * POINTS_PER_INCH
        fig_height_points = fig_height_inches * POINTS_PER_INCH
        ax_width_rate = self.__main_fig.get_axes()[0].get_position().width
        ax_height_rate = self.__main_fig.get_axes()[0].get_position().height
        ax_width_points = fig_width_points * ax_width_rate
        ax_height_points = fig_height_points * ax_height_rate
        square_width_points = ax_width_points / self.__axis_x_len
        square_height_points = ax_height_points / self.__axis_y_len
        marker_size = min([square_width_points, square_height_points]) * self.__marker_size_rate

        return marker_size

    def __calc_annotate_font_size(self) -> float:
        """
        アノテートのフォントサイズをウィンドウサイズから算出

        Returns:
            float: 算出されたフォントサイズ
        """
        return self.__calc_plot_marker_size() * self.__annotate_font_rate

    def __load_parameter_str(self, parameter_str:str) -> list:
        """
        各性能パラメタが取り得る値の数が記述された 1 行の文字列からリストを生成

        Args:
            parameter_str (str): 各性能パラメタが取り得る値の数が記述された 1 行の文字列

        Returns:
            list: 各性能パラメタが取り得る値の数リスト
        """

        parameter_lens = list()

        for n in parameter_str.split(','):
            parameter_lens.append(int(n))

        return parameter_lens

    def __create_heat_map_data(self) -> tuple:
        """
        メンバ変数の各データから，ヒートマップに使用する 2 次元配列データを生成．
        探索空間とヒートマップの座標対応関係もここで生成

        Returns:
            tuple: ヒートマップデータ (list 型) と対応関係 ({(coordinate):(int, int)} 型) のタプル
        """

        heatmap_coordinates = dict()   # {(coordinate):(int, int)}
        heat_map = list()
        for i in range(len(self.__axis_y)):
            heat_map.append(list())
            for j in range(len(self.__axis_x)):
                tmp_coordinate = list()
                for k in range(len(self.__parameter_lens)):
                    if self.__axis_x[j][k] != -1:
                        tmp_coordinate.append(self.__axis_x[j][k])
                    else:
                        tmp_coordinate.append(self.__axis_y[i][k])
                tmp = self.__space_values[tuple(tmp_coordinate)]
                heat_map[i].append(tmp)
                heatmap_coordinates[tuple(tmp_coordinate)] = ([j], [i])

        return (heat_map, heatmap_coordinates)



class _ViewerMenu(tk.Menu):
    def __init__(self, master):
        super().__init__(master)

        # コマンド
        self._comm_load_search_space = None
        self._comm_load_dsice_log = None
        self._comm_open_axis_config_window = None
        self._comm_open_suggest_window = None
        self._comm_open_dsp_window = None
        self._comm_open_measured_window = None
        self._comm_open_version_window = None

        # 配置
        self.__file_menu = tk.Menu(self, tearoff=False)
        self.__file_menu.add_command(label='探索空間ファイル読み込み', command=self.__comm_load_search_space)
        self.__file_menu.add_command(label='DSICE ログファイル読み込み', command=self.__comm_load_dsice_log)
        self.add_cascade(label='ファイル', menu=self.__file_menu)
        self.__view_menu = tk.Menu(self, tearoff=False)
        self.__view_menu.add_command(label='描画軸設定', command=self.__comm_open_axis_config_window)
        self.__view_menu.add_separator()
        self.__view_menu.add_command(label='候補点ウィンドウ', command=self.__comm_open_suggest_window)
        self.__view_menu.add_command(label='d-Spline 近似ウィンドウ', command=self.__comm_open_dsp_window)
        self.__view_menu.add_command(label='実測座標表示ウィンドウ', command=self.__comm_open_measured_window)
        self.add_cascade(label='表示', menu=self.__view_menu)
        self.__other_menu = tk.Menu(self, tearoff=False)
        self.__other_menu.add_command(label='環境設定')
        self.__other_menu.add_separator()
        self.__other_menu.add_command(label='バージョン情報', command=self.__comm_open_version_window)
        self.add_cascade(label='その他', menu=self.__other_menu)

        # 初期状態設定
        self.__file_menu.entryconfig('DSICE ログファイル読み込み', state='disabled')
        self.__view_menu.entryconfig('描画軸設定', state='disabled')
        self.__view_menu.entryconfig('候補点ウィンドウ', state='disabled')
        self.__view_menu.entryconfig('d-Spline 近似ウィンドウ', state='disabled')
        self.__view_menu.entryconfig('実測座標表示ウィンドウ', state='disabled')

    def __comm_load_search_space(self) -> None:
        self._comm_load_search_space()

    def __comm_load_dsice_log(self) -> None:
        self._comm_load_dsice_log()

    def __comm_open_axis_config_window(self) -> None:
        self._comm_open_axis_config_window()

    def __comm_open_suggest_window(self) -> None:
        self._comm_open_suggest_window()

    def __comm_open_dsp_window(self) -> None:
        self._comm_open_dsp_window()

    def __comm_open_measured_window(self) -> None:
        self._comm_open_measured_window()

    def __comm_open_version_window(self) -> None:
        self._comm_open_version_window()

    def _change_state(self, menu_elm:str, state:str) -> None:
        """
        メニュー要素のステータスを変更

        Args:
            menu_elm (str): 変更するメニュー要素
            state (str): 変更後ステータス
        """
        
        if menu_elm == 'DSICE ログファイル読み込み':
            self.__file_menu.entryconfig('DSICE ログファイル読み込み', state=state)
        else:
            self.__view_menu.entryconfig(menu_elm, state=state)

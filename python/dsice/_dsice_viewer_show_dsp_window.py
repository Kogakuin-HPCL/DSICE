import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from dsice.equally_spaced_d_spline import EquallySpacedDSpline as dsp

class _DspWindow(tk.Toplevel):
    def __init__(self, master, bg:str, space_values:dict, line_coordinates:list, measured_coordinates:list, plot_loop:int) -> None:
        super().__init__(master=master)

        self.__window_bg_color = bg
        self.__space_values = space_values
        self.__line_coordinates = line_coordinates
        self.__measured_coordinates = measured_coordinates

        self.title('d-Spline 近似')
        self.geometry("500x500")
        self.configure(bg=self.__window_bg_color)

        self.__create_canvas()
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        self.bind('<Destroy>', self.__closed_window_event)

        self.__sample_x = None      # [float]
        self.__sample_y = None      # [float]
        self.__dsp_x = None         # [float]
        self.__dsp_y_list = None    # [[float]]
        self.__measured_x_list = None   # [[float]]
        self.__measured_y_list = None   # [[float]]

        self.__create_graph_lines()
        self._plot(plot_loop)

    def __closed_window_event(self, event) -> None:
        plt.close()

    def __create_canvas(self) -> None:
        """
        グラフ描画用キャンバスを作成
        """
        self.__main_fig = plt.figure()
        self.__main_canvas = FigureCanvasTkAgg(self.__main_fig, master=self)
        self.__main_canvas.draw()
        self.__main_canvas.get_tk_widget().grid(row=0, column=0, sticky=tk.NSEW)

    def __create_graph_lines(self) -> None:
        """
        各ループでの近似状態を作成し，保存
        """

        self.__sample_x = list(range(1, len(self.__line_coordinates) + 1))
        self.__sample_y = list()
        self.__dsp_x = list()
        self.__dsp_y_list = list()
        self.__measured_x_list = list()
        self.__measured_y_list = list()

        # 対象直線の真値を取得
        for c in self.__line_coordinates:
            self.__sample_y.append(self.__space_values[tuple(c)])

        # 各ループの状態を作成
        d_spline = dsp(len(self.__line_coordinates))
        for i in range(d_spline.get_marker_num()):
            self.__dsp_x.append(float(i + 1) / 3)
        measured_x = list()
        measured_y = list()
        for i in range(len(self.__measured_coordinates)):
            for j in range(len(self.__measured_coordinates[i])):
                if self.__measured_coordinates[i][j] in self.__line_coordinates:
                    idx = self.__line_coordinates.index(self.__measured_coordinates[i][j])
                    d_spline.update_one(idx, self.__sample_y[idx])
                    measured_x.append(idx + 1)
                    measured_y.append(self.__sample_y[idx])

            self.__dsp_y_list.append(d_spline.get_marker_values())
            self.__measured_x_list.append(measured_x[:])
            self.__measured_y_list.append(measured_y[:])

    def _plot(self, plot_loop:int) -> None:
        """
        指定ループの時点での d-Spline 近似状態を表示

        Args:
            plot_loop (int): 表示するループ (1 オリジン)
        """

        self.__main_fig.clear()
        ax = self.__main_fig.add_subplot(111)
        ax.scatter(self.__sample_x, self.__sample_y)
        ax.plot(self.__dsp_x, self.__dsp_y_list[plot_loop - 1])
        ax.scatter(self.__measured_x_list[plot_loop - 1], self.__measured_y_list[plot_loop - 1], c='r')
        ax.set_xlabel('Line Index')
        ax.set_ylabel('Performance')
        self.__main_canvas.draw()

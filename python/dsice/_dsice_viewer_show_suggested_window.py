import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

class _ShowSuggestedWindow(tk.Toplevel):
    def __init__(self, font:tuple, bg:str, marker_size_rate:float, color_bar_type:str, annotate_font_rate:float):
        super().__init__()

        # 基本設定
        self._basic_font = font
        self._window_bg_color = bg
        self.color_bar_type = color_bar_type
        self.marker_size_rate = marker_size_rate
        self.annotate_font_rate = annotate_font_rate

        self.title('候補点 (実測優先度)')
        self.geometry("500x500")
        self.configure(bg=self._window_bg_color)

        self._create_canvas()
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)

        # self.protocol('WM_DELETE_WINDOW', self._closed_window_event)
        self.bind('<Destroy>', self._closed_window_event)
        self.bind('<Configure>', self._window_size_changed_event)

        # メンバ変数の初期化
        self.plot_objs = list() # [matplotlib.lines.Line2D]
        self.annotate_objs = list() # [matplotlib.text.Annotation]
        self._last_width = self.winfo_width()
        self._last_height = self.winfo_height()
        self.axis_x_len = 0
        self.axis_y_len = 0
        self.heat_map = None            # [[float]]
        self.heatmap_coordinates = None # {(coordinate), ([x:int], [y:int])}

    def _closed_window_event(self, event) -> None:
        plt.close()

    def _window_size_changed_event(self, event) -> None:
        if event.width != self._last_width or event.height != self._last_height:
            for plot_obj in self.plot_objs:
                plot_obj.set_markersize(self._calc_plot_marker_size())
            for text_obj in self.annotate_objs:
                text_obj.set_fontsize(self._calc_annotate_font_size())
            self._last_width = event.width
            self._last_height = event.height

    def _create_canvas(self) -> None:
        self.fig = plt.figure()
        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.draw()
        self.canvas.get_tk_widget().grid(row=0, column=0, sticky=tk.NSEW)

    def _calc_plot_marker_size(self) -> float:
        POINTS_PER_INCH = 72 # DTP の基準ポイント (matplotlib の引数が DTP などの「ポイント」を使っているらしい)
        fig_width_inches, fig_height_inches = self.fig.get_size_inches()
        fig_width_points = fig_width_inches * POINTS_PER_INCH
        fig_height_points = fig_height_inches * POINTS_PER_INCH
        ax_width_rate = self.fig.get_axes()[0].get_position().width
        ax_height_rate = self.fig.get_axes()[0].get_position().height
        ax_width_points = fig_width_points * ax_width_rate
        ax_height_points = fig_height_points * ax_height_rate
        square_width_points = ax_width_points / self.axis_x_len
        square_height_points = ax_height_points / self.axis_y_len
        marker_size = min([square_width_points, square_height_points]) * self.marker_size_rate
        return marker_size

    def _calc_annotate_font_size(self) -> float:
        return self._calc_plot_marker_size() * self.annotate_font_rate

    def _set_heat_map(self, heat_map:list, heatmap_coordinates:dict) -> None:
        self.heat_map = heat_map
        self.axis_x_len = len(heat_map[0])
        self.axis_y_len = len(heat_map)
        self.heatmap_coordinates = heatmap_coordinates

    def _update_heat_map(self) -> None:
        x_tick = list()
        y_tick = list()
        if len(self.heat_map[0]) < 11:
            for i in range(len(self.heat_map[0])):
                x_tick.append(i)
        else:
            for i in range(11):
                x_tick.append(int(len(self.heat_map[0]) - 1) * (i / 10))
        if len(self.heat_map) < 11:
            for i in range(len(self.heat_map)):
                y_tick.append(i)
        else:
            for i in range(11):
                y_tick.append(int(len(self.heat_map) - 1) * (i / 10))
        self.fig.clear()
        ax = self.fig.add_subplot(111)
        ax.imshow(self.heat_map, cmap=self.color_bar_type, origin='lower')
        ax.set_xticks(x_tick)
        ax.set_yticks(y_tick)
        self.canvas.draw()

    def _plot_suggested(self, suggested:list) -> None:
        self.plot_objs = list()
        self.annotate_objs = list()
        self._update_heat_map()
        ax = self.fig.get_axes()[0]
        for i in range(len(suggested)):
            s = suggested[i]
            if tuple(s) in self.heatmap_coordinates:
                x, y = self.heatmap_coordinates[tuple(s)]
                plot_obj, = ax.plot(x, y, marker='o', color='white', markeredgecolor='white', markersize=self._calc_plot_marker_size(), linestyle='None')
                self.plot_objs.append(plot_obj)
                annotate_obj = ax.annotate(f'{i + 1}', xy=(x[0], y[0]), ha='center', va='center', fontsize=self._calc_annotate_font_size())
                self.annotate_objs.append(annotate_obj)
        self.canvas.draw()

import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

class _DspEntryWindow(tk.Toplevel):
    def __init__(self, master, font:tuple, bg:str, params_list:list):
        super().__init__(master=master)

        # 基本設定
        self._master = master
        self._parameter_num = len(params_list)
        self._params_list = params_list
        self._basic_font = font
        self._window_bg_color = bg

        self.title('表示する直線を選択')
        self.geometry("900x800")
        self.configure(bg=self._window_bg_color)

        # メンバ変数
        self.selection_objs = list()    # [[Label, Combobox]]

        # ウィジェット配置
        self._create_widgets()

    def _cancel_bt_clicked(self):
        self.destroy()

    def _ok_bt_clicked(self):
        coordinate1 = list()
        coordinate2 = list()
        for i in range(self._parameter_num):
            coordinate1.append(int(self.selection_objs[i][1].get()))
            coordinate2.append(int(self.selection_objs[i][2].get()))

        if coordinate1 == coordinate2:
            messagebox.showerror('入力値エラー', '指定する 2 点は別々でなければなりません')
            return

        self._master._open_dsp_window(coordinate1, coordinate2)
        self.destroy()

    def _create_widgets(self):

        # ダミー
        dummy_label1 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label1.grid(row=0, column=0)

        # OK, Cancel
        self._cancel_bt = tk.Button(self, text='キャンセル', font=self._basic_font, justify=tk.CENTER, width=10, command=self._cancel_bt_clicked)
        self._cancel_bt.grid(row=1, column=1)
        self._ok_bt = tk.Button(self, text='表示', font=self._basic_font, justify=tk.CENTER, width=10, command=self._ok_bt_clicked)
        self._ok_bt.grid(row=1, column=3)

        # ダミー
        dummy_label2 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label2.grid(row=2, column=0)

        # ダミー
        dummy_label3 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label3.grid(row=3, column=0)

        # 設定項目タイトル
        self.select_axis_x_label = tk.Label(self, text='パラメタ', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=4, column=1)
        self.select_axis_x_label = tk.Label(self, text='1点目', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=4, column=2)
        self.select_axis_x_label = tk.Label(self, text='2点目', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=4, column=3)

        # ダミー
        dummy_label4 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label4.grid(row=5, column=0)

        # 設定内容
        for i in range(self._parameter_num):
            tmp_row = 6 + i
            pl = tk.Label(self, text=f'パラメタ{i}', justify='center', bg=self._window_bg_color, font=self._basic_font)
            pl.grid(row=tmp_row, column=1)
            cb1 = ttk.Combobox(self, values=list(range(self._params_list[i])), width=10, height=3, justify='center', state='readonly', font=self._basic_font)
            cb1.set(0)
            cb1.grid(row=tmp_row, column=2)
            cb2 = ttk.Combobox(self, values=list(range(self._params_list[i])), width=10, height=3, justify='center', state='readonly', font=self._basic_font)
            cb2.set(0)
            cb2.grid(row=tmp_row, column=3)
            self.selection_objs.append([pl, cb1, cb2])

        # ダミー
        dummy_label5 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label5.grid(row=6, column=4)

        # 位置調整
        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1)
        self.columnconfigure(3, weight=1)
        self.columnconfigure(4, weight=1)

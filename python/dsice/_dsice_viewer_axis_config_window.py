import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

class _AxisConfigWindow(tk.Toplevel):
    def __init__(self, master, param_num:int, font:tuple, bg:str, usage:list=None, start:list=None, step:list=None):
        super().__init__(master=master)

        # 基本設定
        self._master = master
        self._parameter_num = param_num
        self._basic_font = font
        self._window_bg_color = bg

        self.title('軸の設定')
        self.geometry("900x800")
        self.configure(bg=self._window_bg_color)

        # メンバ変数
        if usage == None:
            self.usage = list() # [int]
            for i in range(self._parameter_num):
                if i == 0:
                    self.usage.append(0)
                elif i == 1:
                    self.usage.append(1)
                else:
                    self.usage.append(2)
        else:
            self.usage = usage
        if start == None:
            self.start = list() # [int]
            for i in range(self._parameter_num):
                self.start.append(0)
        else:
            self.start = start
        if step == None:
            self.step = list()  # [int]
            for i in range(self._parameter_num):
                self.step.append(1)
        else:
            self.step = step
        self.selection_objs = list()    # [[Label, Combobox, Entry, Entry]]

        # ウィジェット配置
        self._create_widgets()

    def _cancel_bt_clicked(self):
        self.destroy()

    def _ok_bt_clicked(self):
        usage = list()
        start = list()
        step = list()
        for i in range(self._parameter_num):
            if self.selection_objs[i][1].get() == 'X軸':
                usage.append(0)
            elif self.selection_objs[i][1].get() == 'Y軸':
                usage.append(1)
            else:
                usage.append(2)
            start.append(int(self.selection_objs[i][2].get()))
            step.append(int(self.selection_objs[i][3].get()))
        self._master._config_axes(usage, start, step)
        self.destroy()

    def _create_widgets(self):

        # ダミー
        dummy_label1 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label1.grid(row=0, column=0)

        # OK, Cancel
        self._cancel_bt = tk.Button(self, text='キャンセル', font=self._basic_font, justify=tk.CENTER, width=10, command=self._cancel_bt_clicked)
        self._cancel_bt.grid(row=1, column=0, columnspan=3)
        self._ok_bt = tk.Button(self, text='設定', font=self._basic_font, justify=tk.CENTER, width=10, command=self._ok_bt_clicked)
        self._ok_bt.grid(row=1, column=3, columnspan=3)

        # ダミー
        dummy_label2 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label2.grid(row=2, column=0)

        # リセットボタン
        self.reset_bt = tk.Button(self, text='リセット', font=self._basic_font, justify=tk.CENTER, width=10, command=self._reset_values)
        self.reset_bt.grid(row=3, column=0, columnspan=6)

        # ダミー
        dummy_label3 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label3.grid(row=4, column=0)

        # 設定項目タイトル
        self.select_axis_x_label = tk.Label(self, text='パラメタ', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=5, column=1)
        self.select_axis_x_label = tk.Label(self, text='軸への採用', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=5, column=2)
        self.select_axis_x_label = tk.Label(self, text='開始', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=5, column=3)
        self.select_axis_x_label = tk.Label(self, text='区間幅', bg=self._window_bg_color, font=self._basic_font, justify=tk.CENTER, width=10)
        self.select_axis_x_label.grid(row=5, column=4)

        # ダミー
        dummy_label4 = tk.Label(self, text=' ', bg=self._window_bg_color)
        dummy_label4.grid(row=6, column=0)

        # 設定内容
        for i in range(self._parameter_num):
            tmp_row = 7 + i
            pl = tk.Label(self, text=f'パラメタ{i}', justify='center', bg=self._window_bg_color, font=self._basic_font)
            pl.grid(row=tmp_row, column=1)
            cb = ttk.Combobox(self, values=['X軸', 'Y軸', '固定'], width=10, height=3, justify='center', state='readonly', font=self._basic_font)
            if self.usage[i] == 0:
                cb.set('X軸')
            elif self.usage[i] == 1:
                cb.set('Y軸')
            else:
                cb.set('固定')
            cb.grid(row=tmp_row, column=2)
            start_entry = tk.Entry(self, font=self._basic_font, width=10, justify='center', relief='solid')
            start_entry.insert(0, f'{self.start[i]}')
            start_entry.grid(row=tmp_row, column=3)
            step_entry = tk.Entry(self, font=self._basic_font, width=10, justify='center', relief='solid')
            step_entry.insert(0, f'{self.step[i]}')
            step_entry.grid(row=tmp_row, column=4)
            self.selection_objs.append([pl, cb, start_entry, step_entry])

        # 位置調整
        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1)
        self.columnconfigure(3, weight=1)
        self.columnconfigure(4, weight=1)
        self.columnconfigure(5, weight=1)

    def _reset_values(self):
        if messagebox.askyesno('リセットの確認', '軸設定を初期状態に戻しますか？'):
            for i in range(self._parameter_num):
                if i == 0:
                    self.selection_objs[i][1].set('X軸')
                elif i == 1:
                    self.selection_objs[i][1].set('Y軸')
                else:
                    self.selection_objs[i][1].set('固定')
                self.selection_objs[i][2].delete(0, tk.END)
                self.selection_objs[i][2].insert(0, '0')
                self.selection_objs[i][3].delete(0, tk.END)
                self.selection_objs[i][3].insert(0, '1')

import tkinter as tk

class _ShowMeasuredWindow(tk.Toplevel):
    def __init__(self, font:tuple, bg:str):
        super().__init__()

        # 基本設定
        self._basic_font = font
        self._window_bg_color = bg

        self.title('実測点')
        self.geometry("300x500")
        self.configure(bg=self._window_bg_color)

        self._create_widgets()
        self.columnconfigure(0, weight=1)

    def _create_widgets(self):

        # ダミー
        dummy_label1 = tk.Label(self, text='', font=self._basic_font, bg=self._window_bg_color)
        dummy_label1.grid(row=0, column=0)

        # ビュー
        self.view_label = tk.Label(self, font=self._basic_font, bg=self._window_bg_color)
        self.view_label.grid(row=1, column=0)

    def _view_measured(self, measured:list) -> None:
        tmp = ''
        for coordinate in measured:
            tmp += f'{coordinate}'
        self.view_label.config(text=tmp)

import tkinter as tk
import webbrowser

class _VersionWindow(tk.Toplevel):
    def __init__(self, font:tuple, bg:str):
        super().__init__()

        # バージョン情報設定
        self.version = '1.0.0'
        self.date = '2025/2/17'

        # 基本設定
        self._basic_font = font
        self._link_font = (self._basic_font[0], self._basic_font[1], 'underline')
        self._window_bg_color = bg

        self.title('バージョン情報')
        self.geometry("600x400")
        self.configure(bg=self._window_bg_color)
        self.resizable(False, False)

        # バージョン掲載
        dummy_label1 = tk.Label(self, text='', bg=self._window_bg_color, font=('Helvetica', 20))
        dummy_label1.grid(row=0, column=1)
        
        title_label = tk.Label(self, text='DSICE Viewer', font=('Helvetica', 20), bg=self._window_bg_color, justify='center')
        title_label.grid(row=1, column=1, sticky=tk.EW)

        dummy_label2 = tk.Label(self, text='', bg=self._window_bg_color, font=('Helvetica', 20))
        dummy_label2.grid(row=2, column=0)

        version_label = tk.Label(self, text=f'バージョン : {self.version}\n更新日 : {self.date}', font=self._basic_font, bg=self._window_bg_color, justify='left')
        version_label.grid(row=3, column=1, sticky=tk.W)

        dummy_label3 = tk.Label(self, text='', bg=self._window_bg_color, font=('Helvetica', 20))
        dummy_label3.grid(row=4, column=0)

        license_label = tk.Label(self, text='DSICE および DSICE Viewer は\n工学院大学先進ソフトウェア研究室により開発されました。', font=self._basic_font, bg=self._window_bg_color, justify='left')
        license_label.grid(row=5, column=1, sticky=tk.W)

        dummy_label4 = tk.Label(self, text='', bg=self._window_bg_color, font=('Helvetica', 20))
        dummy_label4.grid(row=6, column=0)

        link_label1 = tk.Label(self, text='研究室 Web サイト', font=self._link_font, fg='blue', bg=self._window_bg_color, justify='left', cursor='hand2')
        link_label1.bind("<Button-1>", lambda e:webbrowser.open('https://hpcl.info.kogakuin.ac.jp/'))
        link_label1.grid(row=7, column=1, sticky=tk.W)
        link_label1 = tk.Label(self, text='GitHub', font=self._link_font, fg='blue', bg=self._window_bg_color, justify='left', cursor='hand2')
        link_label1.grid(row=8, column=1, sticky=tk.W)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=2)
        self.columnconfigure(2, weight=1)

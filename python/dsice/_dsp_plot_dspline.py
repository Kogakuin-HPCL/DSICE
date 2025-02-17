import numpy as np

# print(np.get_printoptions())
np.set_printoptions(precision=2, suppress=True)

class Dsp:
    def __init__(self,N,ALPHA=0.1):
        self.N = N
        self.ALPHA = ALPHA
        self.n = 3 * self.N + 2
        self.is_added = np.full(self.N, False)
        self.b = np.zeros(self.n)
        self.f = np.zeros(self.n)
        self.para_f = np.zeros(self.N)
        self.Z = np.zeros(3*self.n)
        for i in range(self.n-2):
            self.Z[3 * i] = self.ALPHA
            self.Z[3 * i + 1] = -2 * self.ALPHA
            self.Z[3 * i + 2] = self.ALPHA
        self.fd = np.zeros(self.N)
        self.cnt = 0
        self.nxt_i = -1
        self.min_i = -1

        self.min_cnt = 0
        self.added_i = []
        self.added_v = []

    # 標本点追加
    def add(self,i,y):
        if self.is_added[i]:
            return
        self.is_added[i] = True
        self.cnt += 1
        self.givens(i,y)
        
        self.added_i.append(i)
        self.added_v.append(y)

    # d-Spline更新
    def update(self):
        if self.cnt < 4:
            return
        self.calc_f()
        self.srch_nxt()
        # self.check() 


    # Zf = bのZの一番下の行の要素を全て0にする
    def givens(self,i,y):
        onz0 = 1.0 # 0にする要素
        onz1 = 0.0 # onz0の右隣の要素
        # onz0, onz1, 0 ... -> 0, onz0, onz1, 0,... -> 0, 0, onz0, onz1, 0, ... ->

        for k in range(3*i+2, self.n): # k: marker_index

            if onz0 == onz1 == 0:
                break
            if onz0 == 0 and onz1 != 0:
                onz0, onz1 = onz1, 0 
                continue
            
            # 回転行列Pのcとsを計算
            r = np.sqrt(onz0 * onz0 + self.Z[3*k] * self.Z[3*k])
            c = self.Z[3*k] / r
            s = onz0 / r

            # Zf = b の右辺にPをかける
            new_b =  c * self.b[k] + s * y
            new_y = -s * self.b[k] + c * y
            self.b[k], y = new_b, new_y

            # Zf = b の左辺にPをかける
            self.Z[3*k] = c * self.Z[3*k] + s * onz0

            new_Z1    =  c * self.Z[3 * k + 1] + s * onz1
            new_onz0  = -s * self.Z[3 * k + 1] + c * onz1
            self.Z[3 * k + 1], onz0 = new_Z1, new_onz0

            new_Z2    =  c * self.Z[3 * k + 2]
            new_onz1  = -s * self.Z[3 * k + 2]
            self.Z[3 * k + 2], onz1 = new_Z2, new_onz1

    def calc_f(self):
        # 後退代入でfを求める
        self.f[self.n-1] = self.b[self.n-1] / self.Z[3*(self.n-1)]
        self.f[self.n-2] = (self.b[self.n-2] - self.Z[3*(self.n-2)+1] * self.f[self.n-1])/self.Z[3*(self.n-2)]
        for i in range(self.n-3,-1,-1):
            self.f[i] = (self.b[i]-self.Z[3*i+1]*self.f[i+1]-self.Z[3*i+2]*self.f[i+2])/self.Z[3*i]

        # パラメタのとこだけとってくる
        self.para_f = self.f[2::3]
        # 二階差分
        for i in range(1, self.N-1):
            self.fd[i] = abs(self.para_f[i-1] -2*self.para_f[i] + self.para_f[i+1])


    def check(self):
        Z2 = np.zeros((self.n-2+self.cnt,self.n))
        b2 = np.zeros(self.n-2+self.cnt)
        for i in range(self.n-2):
            Z2[i][i:i+3] = [self.ALPHA, -2*self.ALPHA, self.ALPHA]
        for i in range(self.cnt):
            Z2[self.n-2 + i][3 * self.added_i[i] + 2] = 1
            b2[self.n-2 + i] = self.added_v[i]

        f2 = np.linalg.lstsq(Z2,b2,rcond=None)[0]

        for i in range(self.n):
            if abs(self.f[i] - f2[i]) > 1e-5:
                print("fail")
                exit()
        print("succeed")


    def srch_nxt(self):
        latest_min_i = self.min_i # 直前の最小点

        # 最小点を探索
        self.min_i = np.argmin(self.para_f)

        # 最小点が追加済みなら二階差分が最大の点を探索
        if self.is_added[self.min_i]: 
            self.nxt_i = np.argmax(np.where(self.is_added, -1, self.fd))
        else:
            self.nxt_i = self.min_i

        # 最小点が連続何回選ばれているかカウント
        if latest_min_i == self.min_i:
            self.min_cnt += 1
        else:
            self.min_cnt = 1

    def get_f(self):
        return self.f.copy()

    def get_para_f(self):
        return self.para_f.copy()

    def get_fd(self):
        return self.fd[1:self.N-1].copy()

    def get_nxt(self):
        return self.nxt_i
    
    def get_min(self):
        return self.min_i

    def get_min_cnt(self):
        return self.min_cnt

    def get_added_i(self):
        return self.added_i.copy()
    
    def get_added_v(self):
        return self.added_v.copy()

    # update()しなくても最新を返すの注意
    def get_current(self):
        return self.added_i[-1]


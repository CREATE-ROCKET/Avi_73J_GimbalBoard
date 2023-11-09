import pandas as pd
import matplotlib.pyplot as plt

file_path = 'logs/device-monitor-231110-020033.csv'  # CSVファイルのパスを指定してください

# CSVファイルを読み込む
df = pd.read_csv(file_path, header=None)

# 第一列をミリ秒に変換
df[0] = df[0] / 1000

# グラフの作成
plt.figure(figsize=(10, 5))
for col in df.columns[1:]:
    plt.plot(df[0], df[col], label=f'Column {col}')

plt.xlabel('CPU Time (milliseconds)')
plt.ylabel('Values')
plt.title('Graph of CPU Time vs. Other Columns')
plt.legend()
plt.grid(True)
plt.show()
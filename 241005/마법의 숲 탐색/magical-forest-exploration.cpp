#include <bits/stdc++.h>
#define ARR_MAX 7

using namespace std;

typedef pair<int, int> pii;

int R, C, K;
// 0,0 기준으로 가정
int arr[ARR_MAX + 3][ARR_MAX]; // 최초 시작은 3이 더해져야 함
bool isExit[ARR_MAX + 3][ARR_MAX];
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};
int ans = 0;

bool inRange(int y, int x)
{
    return 3 <= y && y < R + 3 && 0 <= x && x < C;
}

void resetMap()
{
    for (int i = 0; i < R + 3; i++)
    {
        for (int j = 0; j < C; j++)
        {
            arr[i][j] = 0;
            isExit[i][j] = false;
        }
    }
}

// 시작 y 와 시작 x
int bfs(int y, int x)
{
    queue<pii> q;
    bool visited[ARR_MAX + 3][ARR_MAX];

    for (int i = 0; i < R + 3; i++)
    {
        for (int j = 0; j < C; j++)
        {
            visited[i][j] = false;
        }
    }

    // 지금 있는 위치로 초기화
    int result = y;
    q.push({y, x});
    visited[y][x] = true;
    while (!q.empty())
    {
        int py = q.front().first;
        int px = q.front().second;

        q.pop();
        for (int k = 0; k < 4; k++)
        {
            int ny = py + dy[k];
            int nx = px + dx[k];

            if (inRange(ny, nx))
            {
                if (visited[ny][nx] == false && arr[ny][nx] != 0)
                {
                    // 같은 골렘 내부이거나 현재 위치가 탈출구이거나
                    if (arr[py][px] == arr[ny][nx] || isExit[py][px])
                    {
                        visited[ny][nx] = true;
                        q.push({ny, nx});

                        result = max(result, ny);
                    }
                }
            }
        }
    }

    return result;
}

// 골렘의 중심이 (y,x) 에 있을 수 있는지 확인
// 북쪽에서 남쪽으로 내려와야하므로 중심이 (y, x)에 위치할때의 범위와 (y-1, x)에 위치할 때의 범위 모두 확인합니다 (동쪽으로 회전, 서쪽으로 회전을 모두 생각하는 것!)
// down 함수가 재귀로 이루어져 있기에 자기 자신이 차있다(?) 라는 개념은 생각하지 않아도 됨
bool canGo(int y, int x)
{
    bool flag = 0 <= x - 1 && x + 1 < C && y + 1 < R + 3;
    flag = flag && (arr[y - 1][x - 1] == 0);
    flag = flag && (arr[y - 1][x] == 0);
    flag = flag && (arr[y - 1][x + 1] == 0);
    flag = flag && (arr[y][x - 1] == 0);
    flag = flag && (arr[y][x] == 0);
    flag = flag && (arr[y][x + 1] == 0);
    flag = flag && (arr[y + 1][x] == 0);

    return flag;
}

// 내려가기 함수
void down(int y, int x, int d, int id)
{
    if (canGo(y + 1, x))
    {
        down(y + 1, x, d, id);
        return;
    }

    if (canGo(y + 1, x - 1))
    {
        down(y + 1, x - 1, (d + 3) % 4, id);
        return;
    }
    if (canGo(y + 1, x + 1))
    {
        down(y + 1, x + 1, (d + 1) % 4, id);
        return;
    }
    // 위의 3가지를 다 못함 -> 내려올만큼 내려왔음

    // 골렘의 몸의 일부가 밖으로 나와 있을때
    if (!inRange(y - 1, x - 1) || !inRange(y + 1, x + 1))
    {
        resetMap();
        return;
    }

    // 골렘이 숲 속에 정착
    arr[y][x] = id;
    for (int k = 0; k < 4; k++)
        arr[y + dy[k]][x + dx[k]] = id;

    isExit[y + dy[d]][x + dx[d]] = true;

    ans += bfs(y, x) - 2;
}

int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    cin >> R >> C >> K;

    for (int id = 1; id <= K; id++)
    { // 골렘 번호 id
        int x, d;
        cin >> x >> d; // 골렘의 출발 x좌표, 방향 d를 입력받습니다
        x--;
        down(0, x, d, id);
    }

    cout << ans << endl;
}

/**
 * 어차피 무슨 골렘인지, 이전 정령이 어디서 멈췄는지는 확인할 필요가 없다.
 */
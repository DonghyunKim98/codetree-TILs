#include <bits/stdc++.h>
#define MAX_ARR 51
#define INF 987654321

using namespace std;

typedef pair<int, int> pii;

struct 산타
{
    int y;
    int x;
    int 산타_번호;
    int 점수;
    int is기절;
    bool is탈락;
};

int N, M, P, C, D;
int arr[MAX_ARR][MAX_ARR];

pii 루돌프 = {-1, -1};
vector<산타> 산타_vc;

int dy_루돌프[8] = {0, 1, 0, -1, -1, -1, 1, 1};
int dx_루돌프[8] = {-1, 0, 1, 0, -1, 1, -1, 1};

int dy_산타[4] = {0, 1, 0, -1};
int dx_산타[4] = {-1, 0, 1, 0};

void print산타()
{
    cout << 루돌프.first << ' ' << 루돌프.second << endl;
    for (auto u : 산타_vc)
        cout << u.산타_번호 << ' ' << u.y << ' ' << u.x << ' ' << u.is기절 << ' ' << u.is탈락 << ' ' << u.점수 << endl;

    cout << endl;
}

bool comp산타(산타 a, 산타 b)
{

    if (a.y == b.y)
        return a.x < b.x;

    else
        return a.y < b.y;
}

bool comp산타원상복귀(산타 a, 산타 b)
{
    return a.산타_번호 < b.산타_번호;
}

bool inRange(int y, int x)
{
    return 1 <= y && y <= N && 1 <= x && x <= N;
}

void input()
{
    cin >> N >> M >> P >> C >> D;

    int 루돌프_y, 루돌프_x;
    cin >> 루돌프_y >> 루돌프_x;

    루돌프 = {루돌프_y, 루돌프_x};

    for (int i = 0; i < P; i++)
    {
        int 산타_번호, 산타_y, 산타_x;
        cin >> 산타_번호 >> 산타_y >> 산타_x;

        산타 temp산타;
        temp산타.산타_번호 = 산타_번호;
        temp산타.y = 산타_y;
        temp산타.x = 산타_x;
        temp산타.점수 = 0;
        temp산타.is기절 = false;
        temp산타.is탈락 = false;

        산타_vc.push_back(temp산타);
    }
}

void ouput()
{
    for (auto u : 산타_vc)
        cout << u.점수 << ' ';
}

int calcDistance(int y1, int x1, int y2, int x2)
{
    return pow(y1 - y2, 2) + pow(x1 - x2, 2);
}

void 산타_밀려남(int 산타_idx, int 방향, int power)
{
    // 산타는 밀려남
    int 산타_y = 산타_vc[산타_idx].y;
    int 산타_x = 산타_vc[산타_idx].x;

    산타_y += dy_루돌프[방향] * power;
    산타_x += dx_루돌프[방향] * power;

    // 탈락여부 확인
    if (!inRange(산타_y, 산타_x))
    {
        산타_vc[산타_idx].is탈락 = true;
        return;
    }

    산타_vc[산타_idx].y = 산타_y;
    산타_vc[산타_idx].x = 산타_x;
    // 만약 밀려난 위치에 산타가 있다면 상호작용
    bool flag = false;
    int 충돌된_산타_idx;
    for (int i = 0; i < P; i++)
    {
        if (산타_idx == i || 산타_vc[i].is탈락)
            continue;

        int cur_산타_y = 산타_vc[i].y;
        int cur_산타_x = 산타_vc[i].x;

        if (cur_산타_y == 산타_vc[산타_idx].y && cur_산타_x == 산타_vc[산타_idx].x)
        {
            flag = true;
            충돌된_산타_idx = i;
            break;
        }
    }
    if (flag)
    {
        // 상호작용으로 밀려남
        산타_밀려남(충돌된_산타_idx, 방향, 1);
        return;
    }
}

void 산타_루돌프_충돌by루돌프(int 산타_idx, int 방향)
{
    // 산타는 C 만큼 점수를 얻음
    산타_vc[산타_idx].점수 += C;
    // 산타는 기절함
    산타_vc[산타_idx].is기절 = 2;
    // 산타는 밀려남
    산타_밀려남(산타_idx, 방향, C);
}

void 산타_루돌프_충돌by산타(int 산타_idx, int 방향)
{
    // 산타는 D 만큼 점수를 얻음
    산타_vc[산타_idx].점수 += D;
    // 산타는 기절함
    산타_vc[산타_idx].is기절 = 1;
    // 산타는 반대 방향으로 밀려남
    산타_밀려남(산타_idx, (방향 + 2) % 4, D);
}

int get가장가까운산타idx()
{
    int 가장가까운산타_idx = -1;
    int 가장가까운산타_거리 = INF;
    int 루돌프_y = 루돌프.first, 루돌프_x = 루돌프.second;

    sort(산타_vc.begin(), 산타_vc.end(), comp산타);

    for (int i = 0; i < P; i++)
    {
        if (산타_vc[i].is탈락)
            continue;

        int 산타_y = 산타_vc[i].y;
        int 산타_x = 산타_vc[i].x;

        int 루돌프_산타_거리 = calcDistance(산타_y, 산타_x, 루돌프_y, 루돌프_x);

        가장가까운산타_거리 = min(가장가까운산타_거리, 루돌프_산타_거리);

        if (가장가까운산타_거리 == 루돌프_산타_거리)
            가장가까운산타_idx = i;
    }

    return 가장가까운산타_idx;
}

void move루돌프(int 산타_idx)
{
    int 가장가까운방향;
    int 움직이고나서_가장가까운거리 = INF;

    int 루돌프_y = 루돌프.first, 루돌프_x = 루돌프.second;
    int 산타_y = 산타_vc[산타_idx].y;
    int 산타_x = 산타_vc[산타_idx].x;

    for (int k = 0; k < 8; k++)
    {
        int 루돌프_ny = 루돌프_y + dy_루돌프[k];
        int 루돌프_nx = 루돌프_x + dx_루돌프[k];

        if (!inRange(루돌프_ny, 루돌프_nx))
            continue;

        int 루돌프_움직이고나서_거리 = calcDistance(산타_y, 산타_x, 루돌프_ny, 루돌프_nx);

        움직이고나서_가장가까운거리 = min(움직이고나서_가장가까운거리, 루돌프_움직이고나서_거리);
        if (움직이고나서_가장가까운거리 == 루돌프_움직이고나서_거리)
            가장가까운방향 = k;
    }

    int 최종_루돌프_y = 루돌프_y + dy_루돌프[가장가까운방향];
    int 최종_루돌프_x = 루돌프_x + dx_루돌프[가장가까운방향];

    루돌프 = {최종_루돌프_y, 최종_루돌프_x};

    if (최종_루돌프_y == 산타_y && 최종_루돌프_x == 산타_x)
    {
        산타_루돌프_충돌by루돌프(산타_idx, 가장가까운방향);
    }
}

void move산타(int 산타_idx)
{
    int 루돌프_y = 루돌프.first, 루돌프_x = 루돌프.second;
    int 산타_y = 산타_vc[산타_idx].y;
    int 산타_x = 산타_vc[산타_idx].x;

    int 가장가까운방향 = -1;
    int 움직이고나서_가장가까운거리 = calcDistance(루돌프_y, 루돌프_x, 산타_y, 산타_x);

    for (int k = 0; k < 4; k++)
    {
        int 산타_ny = 산타_y + dy_산타[k];
        int 산타_nx = 산타_x + dx_산타[k];

        if (!inRange(산타_ny, 산타_nx))
            continue;

        // 다른 산타가 있는 지점으로 가는지 확인하고 다른 산타가 있다면 방문하지 않음
        bool flag = false;
        for (int i = 0; i < P; i++)
        {
            if (산타_idx == i || 산타_vc[i].is탈락)
                continue;

            int cur_산타_y = 산타_vc[i].y;
            int cur_산타_x = 산타_vc[i].x;

            if (cur_산타_y == 산타_ny && cur_산타_x == 산타_nx)
            {
                flag = true;
                break;
            }
        }

        if (flag)
            continue;

        int 산타_움직이고나서_거리 = calcDistance(산타_ny, 산타_nx, 루돌프_y, 루돌프_x);

        움직이고나서_가장가까운거리 = min(움직이고나서_가장가까운거리, 산타_움직이고나서_거리);
        if (움직이고나서_가장가까운거리 == 산타_움직이고나서_거리)
            가장가까운방향 = k;
    }

    if (가장가까운방향 == -1)
        return;

    int 최종_산타_y = 산타_y + dy_산타[가장가까운방향];
    int 최종_산타_x = 산타_x + dx_산타[가장가까운방향];

    산타_vc[산타_idx].y = 최종_산타_y;
    산타_vc[산타_idx].x = 최종_산타_x;

    if (루돌프_y == 산타_vc[산타_idx].y && 루돌프_x == 산타_vc[산타_idx].x)
    {
        산타_루돌프_충돌by산타(산타_idx, 가장가까운방향);
    }
}

int main()
{
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    input();

    while (M--)
    {

        int idx = get가장가까운산타idx();
        if (idx != -1)
            move루돌프(idx);

        sort(산타_vc.begin(), 산타_vc.end(), comp산타원상복귀);

        for (int i = 0; i < P; i++)
        {
            if (산타_vc[i].is기절)
            {
                산타_vc[i].is기절--;
                continue;
            }

            if (산타_vc[i].is탈락)
                continue;

            move산타(i);
        }

        bool flag = true;
        for (int i = 0; i < P; i++)
        {
            if (산타_vc[i].is탈락)
                continue;

            flag = false;
            산타_vc[i].점수++;
        }
        if (flag)
            break;

        // cout << idx << endl;
        // print산타();
    }

    ouput();
}
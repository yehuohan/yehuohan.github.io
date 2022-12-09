
clear;
clc;

% ddx + dx + 0.5x = 0
x = 0;          % x初值
dx = 10;        % dx初值
n = 1;          % 下标
time = 0;       % 仿真时间长度
dt = 0.001;     % 积分步长

for i = 1:20000
    ddx = -dx - 0.5 * x;
    dx = dx + ddx * dt;
    x = x + dx * dt;

    p_dx(n) = dx;
    p_x(n) = x;
    n = n + 1;
    time = time + dt;
end

figure(1);
plot(p_x, p_dx);


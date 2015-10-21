function Xout = noNaN(X)

    X(isnan(X)) = 0;
    Xout = X;
end
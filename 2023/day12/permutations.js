function f(N,K) {
    if (N==0)
        return [''];
    if (N==K)
        return ['1'.repeat(K)];
    if (K==0)
        return ['0'.repeat(N)];
    return [...(f(N-1,K-1).map((e)=>'1'+e)),...(f(N-1,K).map((e)=>'0'+e))] }
# entisi
Entropy calculation for ISI trains. Performs binarization of a spike train represented by ISIs
and calculates block entropies H(n), increasing the block order n, and calculating conditional entropies
h(n) = H(n+1) - H(n), as well.

Method for entropy estimation comes from here:
P.Grassberger "Entropy Estimates from Insufficient Samplings" // arXiv:physics/0307138v2 (2008)

Details on block (joint) and conditional entropy see here:
S.Luccioli, T.Kreuz, and A.Torcini "Dynamical response of the Hodgkin-Huxley model
in the high-input regime" // PRE 73, 041902 (2006)

# kf.py updated file for robot estimation assignment
# pset[3]
# (C) 2017 David Feil-Seifer


import numpy as np
import math
from scipy import stats
import scipy.stats

# kf_update: update state estimate [u, sigma] with new control [xdot] and measurement [z]
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t
#			sigma: 2x2 matrix with covariance at time t-1
#			z (int): observed (uncertain) measurement of state (x) at time t
#	returns: [u sigma] updated state with estimate at time t

def kf_update(u, sigma, z):
	z_std_dev = 10
	F = np.matrix([[1,1],[0,1]])
	H = np.matrix([1,0])
	Q = np.matrix([[0,0],[0,1]])
	R = z_std_dev*z_std_dev
	I = np.eye(2)

	z = z + z_std_dev
	u_p = F*u
	sigma_p = F*sigma* F.T + Q
	K = (sigma_p* H.T)/((H*sigma_p*H.T) + R)
	u = u_p + K*(z-H*u_p)
	sigma =(I-K*H)*sigma_p
	return [u, sigma]


# door_update: update estimate of door locations
# 	parameters:
#			u : 2x1 vector with state estimate (x) at time t-1 and control (xdot) at time t-1
#			sigma: 2x2 matrix with covariance at time t-1
#			d (binary): door sensor at time t-1
#			door_dist (array of size 10): probability (0..1) that a door exists at each location (0..9)
#	returns: [door_dist] updated door distribution

def door_update(u, sigma, d, door_dist):
	p_s_d = 0.6
	p_ns_d = 1 - p_s_d
	p_ns_nd =0.8
	p_s_nd = 1 - p_ns_nd
	pos = u[0,0]
	ind =int(pos/100)
	if d == 1:
		pd = door_dist[ind]
		p_nd = 1 - pd
		p_s = p_s_d*pd+p_s_nd* p_nd
		door_dist[ind] = p_s_d*door_dist[ind]/p_s
	else:
		pd = door_dist[ind]
		p_nd = 1 - pd
		p_ns = p_ns_d*pd+p_ns_nd* p_nd
		door_dist[ind] = p_ns_d*door_dist[ind]/p_ns

	return door_dist

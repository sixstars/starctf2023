extern crate libc;
mod md5;

use lazy_static::lazy_static;
use rand::Rng;
use std::collections::HashSet;
use std::io;
use std::ptr;
use std::sync::Mutex;
use std::thread;
use std::time::Duration;

fn handle_alarm(_: i32) {
    println!("Perhaps your input is incorrect, or it is possible that you are not using an ARM CPU with multiple cores. Bye.");
    std::process::exit(0);
}

static mut COUNT: u32 = 20;
static mut TWMM: [u32; 102400000] = [0; 102400000];
static mut AAA: *mut u32 = unsafe { TWMM.as_mut_ptr() };
static mut BBB: u32 = 0;
static mut LOOP: i32 = 0;
static mut succ_flag: i32 = 0;
static mut wmm_count: i32 = 0;

static mut inp: [u8; 40] = [0; 40];
static mut inp2: [u8; 40] = [0; 40];

static CN: usize = 9;
static BN: usize = 5;
static mut matrix: Vec<Vec<f64>> = Vec::new();

lazy_static! {
    static ref P: Mutex<HashSet<usize>> = Mutex::new(HashSet::new());
}

unsafe fn pivot(p: &mut (usize, usize)) -> bool {
    let mut x = 0;
    let mut y = 0;
    let mut cmax = -f64::INFINITY;
    let c_s = matrix[0].clone();
    let mut b_c = Vec::new();
    for i in 0..BN {
        b_c.push(matrix[i][CN - 1]);
    }
    let mut p_set = P.lock().unwrap();
    for i in 0..c_s.len() {
        if cmax < c_s[i] && !p_set.contains(&i) {
            cmax = c_s[i];
            y = i;
        }
    }
    if cmax < 0.0 {
        return false;
    }
    let mut bmin = f64::INFINITY;
    for i in 1..BN {
        let tmp = b_c[i] / matrix[i][y];
        if matrix[i][y] != 0.0 && bmin > tmp && tmp >= 0.0 {
            bmin = tmp;
            x = i;
        }
        if tmp < 0.0 {
            bmin = tmp;
        }
    }
    *p = (x, y);
    if bmin < 0.0 {
        return false;
    }

    let mut t_v: Vec<usize> = Vec::new();
    for it in p_set.iter() {
        t_v.push(*it);
    }

    for it in t_v.iter() {
        if matrix[x][*it] != 0.0 {
            p_set.remove(it);
            break;
        }
    }
    p_set.insert(y);
    true
}

unsafe fn gaussian(p: (usize, usize)) {
    let x = p.0;
    let y = p.1;
    let norm = matrix[x][y];
    for i in 0..CN {
        matrix[x][i] /= norm;
    }
    for i in 0..BN {
        if i != x && matrix[i][y] != 0.0 {
            let tmp_norm = matrix[i][y];
            for j in 0..CN {
                matrix[i][j] = matrix[i][j] - tmp_norm * matrix[x][j];
            }
        }
    }
}

unsafe fn solve() -> f64 {
    let mut t = (0, 0);
    let mut countt = 70;
    while countt > 0 {
        if !pivot(&mut t) {
            break;
        }
        gaussian(t);
        countt -= 1;
    }
    let ret_value = matrix[0][CN - 1];
    if matrix[0][4] == 0.0 && matrix[0][5] == 0.0 && matrix[0][6] == 0.0 && matrix[0][7] == 0.0 {
        return f64::NEG_INFINITY;
    }
    ret_value
}

unsafe fn init_data() {
    let mut vectmp0 = vec![70.0, 65.0, 80.0, 75.0, 0.0, 0.0, 0.0, 0.0, 0.0];
    matrix.push(vectmp0);
    let mut vectmp1 = vec![4.0, 4.0, 3.0, 7.0, 1.0, 0.0, 0.0, 0.0]; //, 90.0];
    matrix.push(vectmp1);
    let mut vectmp2 = vec![6.0, 3.0, 5.0, 4.0, 0.0, 1.0, 0.0, 0.0]; //, 120.0];
    matrix.push(vectmp2);
    let mut vectmp3 = vec![5.0, 2.0, 3.0, 3.0, 0.0, 0.0, 1.0, 0.0]; //, 60.0];
    matrix.push(vectmp3);
    let mut vectmp4 = vec![6.0, 5.0, 1.0, 2.0, 0.0, 0.0, 0.0, 1.0]; //, 100.0];
    matrix.push(vectmp4);
}

unsafe fn place_inp_data() {
    let mut in_arr: Vec<f32> = Vec::with_capacity(5);
    for i in 0..5 {
        let start = i * 4;
        let end = start + 4;
        let bytes_slice = &inp2[start..end];
        let value = match bytes_slice {
            [b0, b1, b2, b3] => {
                let bytes_array: [u8; 4] = [*b0, *b1, *b2, *b3];
                f32::from_le_bytes(bytes_array) // or use f64::from_be_bytes(bytes_array) if big-endian
            }
            _ => panic!("Invalid input"),
        };
        in_arr.push(value);
    }

    matrix[1].push(
        90.0 - in_arr[0] as f64 * matrix[1][0]
            - in_arr[1] as f64 * matrix[1][1]
            - in_arr[2] as f64 * matrix[1][2]
            - in_arr[3] as f64 * matrix[1][3],
    );
    matrix[2].push(
        120.0
            - in_arr[0] as f64 * matrix[2][0]
            - in_arr[1] as f64 * matrix[2][1]
            - in_arr[2] as f64 * matrix[2][2]
            - in_arr[3] as f64 * matrix[2][3],
    );
    matrix[3].push(
        60.0 - in_arr[0] as f64 * matrix[3][0]
            - in_arr[1] as f64 * matrix[3][1]
            - in_arr[2] as f64 * matrix[3][2]
            - in_arr[3] as f64 * matrix[3][3],
    );
    matrix[4].push(
        100.0
            - in_arr[0] as f64 * matrix[4][0]
            - in_arr[1] as f64 * matrix[4][1]
            - in_arr[2] as f64 * matrix[4][2]
            - in_arr[3] as f64 * matrix[4][3],
    );
}

unsafe fn input_data() {
    let mut ss: [u8; 40] = [0; 40];
    let mut t = ss.as_mut_ptr();
    let mut input_string = String::new();

    io::stdin()
        .read_line(&mut input_string)
        .expect("read error");
    let input_bytes = input_string.trim().as_bytes();
    let input_length = input_bytes.len().min(40);
    ss[..input_length].copy_from_slice(&input_bytes[..input_length]);
    // Copy data from t to inp
    inp.copy_from_slice(&ss[..40]);

    for i in 0..40 {
        if (*t.offset(i as isize) > b'0' - 1 && *t.offset(i as isize) < b'9' + 1) {
            *t.offset(i as isize) -= b'0';
        } else if (*t.offset(i as isize) > b'a' - 1 && *t.offset(i as isize) < b'g') {
            *t.offset(i as isize) = *t.offset(i as isize) - b'a' + 0xa;
        }
        if i % 2 == 0 {
            *t.offset((i / 2) as isize) = *t.offset(i as isize) << 4;
        } else {
            *t.offset((i / 2) as isize) =
                *t.offset((i / 2) as isize) | (*t.offset(i as isize) & 0xf);
        }
    }

    inp2.copy_from_slice(&ss[..40]);
    let mut ctx: md5::MD5Context = md5::MD5Context {
        size: 0,
        buffer: [0; 4],
        input: [0; 64],
        digest: [0; 16],
    };
    md5::md5_init(&mut ctx);
    md5::md5_update(&mut ctx, &mut ss, 20);
    md5::md5_finalize(&mut ctx);
    let hash_result2 = "d2edf678c89caf9979ec2b246634d284";
    let mut hash_buf: [u8; 32] = [0; 32];
    for i in 0..16 {
        let u_data1 = (ctx.digest[i] >> 4) & 0xF;
        let u_data2 = ctx.digest[i] & 0xF;
        hash_buf[i * 2] = if u_data1 > 9 {
            u_data1 - 10 + b'a'
        } else {
            u_data1 + b'0'
        };
        hash_buf[i * 2 + 1] = if u_data2 > 9 {
            u_data2 - 10 + b'a'
        } else {
            u_data2 + b'0'
        };
    }
    let hash_result = String::from_utf8_lossy(&hash_buf);
    if hash_result2 == hash_result {
        succ_flag += 1;
    }
}

unsafe fn check_inp_data() -> bool {
    90.0 < matrix[1][CN - 1]
        || 120.0 < matrix[2][CN - 1]
        || 60.0 < matrix[3][CN - 1]
        || 100.0 < matrix[4][CN - 1]
}

unsafe fn check_result() -> bool {
    let mut in_arr: Vec<f32> = Vec::with_capacity(5);
    for i in 0..5 {
        let start = i * 4;
        let end = start + 4;
        let bytes_slice = &inp2[start..end];
        let value = match bytes_slice {
            [b0, b1, b2, b3] => {
                let bytes_array: [u8; 4] = [*b0, *b1, *b2, *b3];
                f32::from_le_bytes(bytes_array) // or use f64::from_be_bytes(bytes_array) if big-endian
            }
            _ => panic!("Invalid input"),
        };
        in_arr.push(value);
    }
    return in_arr[4] as f64
        == in_arr[0] as f64 * matrix[0][0]
            + in_arr[1] as f64 * matrix[0][1]
            + in_arr[2] as f64 * matrix[0][2]
            + in_arr[3] as f64 * matrix[0][3];
}

static mut TEMP_FFF: u8 = 1;

unsafe fn thread1() {
    init_data();
    let mut rng = rand::thread_rng();
    loop {
        for i in 0..COUNT {
            *AAA.offset(i as isize) = 1;
        }
        BBB = 1;
        while ptr::read_volatile(&LOOP) == 0 {}

        AAA = &mut *TWMM
            .as_mut_ptr()
            .offset(rng.gen_range(0..102300000) as isize);
        COUNT = rng.gen_range(0..10);
        for j in 0..COUNT {
            *AAA.offset(j as isize) = 0;
        }
        if (TEMP_FFF == 1) && (succ_flag == 2) {
            let mut vectmp0 = vec![70.0, 65.0, 80.0, 75.0, 0.0, 0.0, 0.0, 0.0, 0.0];
            matrix[0] = vectmp0;
            TEMP_FFF = 0;
        }
        BBB = 0;
        LOOP = 0;
    }
}

unsafe fn thread2() {
    let mut p_a = P.lock().unwrap();
    for i in 0..(BN - 1) {
        p_a.insert(CN - i - 2);
    }
    drop(p_a);
    loop {
        while ptr::read_volatile(&BBB) == 0 {}
        let mut sum: u32 = 0;
        for i in (0..COUNT).rev() {
            sum += *AAA.offset(i as isize);
        }
        if sum != COUNT {
            wmm_count += 1;
            match wmm_count {
                1 => {
                    place_inp_data();
                    if check_inp_data() {
                        wmm_count += 1;
                    }
                }
                2 => {
                    let ret = solve();
                    if ret == 0.0 {
                        succ_flag += 1;
                    }
                }
                3 => {
                    if check_result() {
                        succ_flag += 1;
                    }
                }
                _ => {}
            }
        }
        LOOP = 1;
        while ptr::read_volatile(&LOOP) == 1 {}
    }
}

fn main() {
    use std::os::unix::thread::JoinHandleExt;
    unsafe {
        libc::alarm(180);
        libc::signal(libc::SIGALRM, handle_alarm as usize);
    }
    unsafe {
        input_data();
    }
    let mut th1;
    let mut th2;
    unsafe {
        th1 = thread::spawn(move || thread1());
        th2 = thread::spawn(move || thread2());
    }

    let mut num_cores: i64;
    unsafe {
        num_cores = libc::sysconf(libc::_SC_NPROCESSORS_CONF);
    }
    while unsafe { succ_flag < 3 } {
        let cpuid = rand::thread_rng().gen_range(0..num_cores);
        unsafe {
            let mut cpuset: libc::cpu_set_t = std::mem::zeroed();
            libc::CPU_SET(cpuid as usize, &mut cpuset);
            libc::pthread_setaffinity_np(
                th1.as_pthread_t(),
                std::mem::size_of::<libc::cpu_set_t>(),
                &cpuset,
            );
        }

        let cpuid = rand::thread_rng().gen_range(0..num_cores);
        unsafe {
            let mut cpuset: libc::cpu_set_t = std::mem::zeroed();
            libc::CPU_SET(cpuid as usize, &mut cpuset);
            libc::pthread_setaffinity_np(
                th2.as_pthread_t(),
                std::mem::size_of::<libc::cpu_set_t>(),
                &cpuset,
            );
        }

        thread::sleep(Duration::from_micros(500000));
    }
    unsafe {
        println!("*CTF{{{}}}", String::from_utf8_lossy(&inp));
    }
}

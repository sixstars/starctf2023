use std::mem;
use std::io::Write;
use std::io::BufRead;
use std::io::BufReader;

type Bytes = Vec<u8>;
const N: usize= 100;

fn read_string(s:&mut Bytes){
    let mut reader = BufReader::new(std::io::stdin());
    s.clear();
    reader.read_until(b'\n',s);
    let length = s.len();
    s[length-1] = b'\0';
}

fn read_num() -> usize {
    let mut s = String::new();
    std::io::stdin().read_line(&mut s)
               .expect("Invalid Input");
    let num = s.trim().parse::<usize>().expect("Invalid num");
    return num 
}

fn read_idx() -> usize{
    println!("Index: ");
    let idx = read_num();
    assert!(idx < N);
    return idx;
}

unsafe fn get_by_index(list: &[Bytes], index: isize) -> *const Bytes{
    let list_offset = list.as_ptr();
    list_offset.offset(index)
}

fn add(list: &mut Vec<Bytes>) {
    let mut new_item = Bytes::new();
    println!("content of the new item: ");
    read_string(&mut new_item);
    list.push(new_item);
}

fn edit(list: &mut [Bytes]){
    let idx = read_idx();
    println!("new content: ");
    read_string(&mut list[idx]);
}

fn show(list: &[Bytes]){
    let idx = read_idx();
    println!("The {}th item: ", idx);
    std::io::stdout().write(&list[idx]);
    std::io::stdout().write(b"\n");
}

fn swap(list: &mut [Bytes], ia: usize, ib: usize){
    if ia == ib {
        return;
    }
    unsafe{
        let tmp = get_by_index(&list, ia as isize).read();
        list[ia] = list[ib].clone();
        list[ib] = tmp.clone();
    }
}

fn bubble_east(list: &mut [Bytes]){
    println!("Launching index: ");
    let start: usize = read_num();
    assert!(start < list.len()) ;
    if start == list.len() - 1 {
        return;
    }
    let mut i:usize=start;
    let length: usize = list.len();
    while i<length-1 {
        swap(list,i,i+1);
        i += 1;
    }
}

fn bubble_west(list: &mut [Bytes]){
    println!("Launching index: ");
    let start: usize = read_num();
    assert!(start < list.len()) ;
    if start == 0 {
        return;
    }
    let mut i:usize=start;
    let length: usize = list.len();
    while i>0 {
        swap(list,i-1,i);
        i -= 1
    }
}

fn bubble(list: &mut [Bytes]){
    println!("West/East? (0 is West and 1 is East)");
    let east:usize = read_num();
    if east == 1 {
        bubble_east(list);
    }
    else if east == 0 {
        bubble_west(list);
    }
    else {
        panic!("Invalid direction of bubble!");
    }
}

fn delete(list: &mut [Bytes]){
// fn delete(list: &mut [String]){
    println!("Hey! This is a dummy delete! No one will be hurt!") 
}

fn menu() -> usize {
    println!("-----------------------------------");
    println!("1. add an item.");
    println!("2. Show an item.");
    println!("3. Edit an item.");
    println!("4. bubble.");
    println!("5. Delete an item.");
    println!("-----------------------------------");
    println!("Your choice: ");
    let ch = read_num();
    return ch;
}

fn main() {
    let mut items: Vec<Bytes> = Vec::new();
    loop{ 
        let ch = menu();
        match ch {
            1 => add(&mut items),
            2 => show(&items),
            3 => edit(&mut items),
            4 => bubble(&mut items),
            5 => delete(&mut items),
            _ => println!("Invalid choice."),
        }
    }
}
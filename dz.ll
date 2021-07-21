; ModuleID = 'dz'
source_filename = "dz"

%data_0 = type {}
%data_1 = type { i32, i32*, i32 (i32, i32*)* }
%data_2 = type { i32, i32*, i32 (i32, i32*)* }

define i32 @main() {
entry:
  %0 = alloca %data_0, align 8
  %1 = call i32 @foo(i32 1, %data_0* %0, i32 (i32, i32*)* @closure_2)
  ret i32 %1
}

define i32 @foo(i32 %0, i32* %1, i32 (i32, i32*)* %2) {
entry:
  %3 = alloca %data_1, align 8
  %4 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 0
  store i32 %0, i32* %4, align 4
  %5 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 1
  store i32* %1, i32** %5, align 8
  %6 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 2
  store i32 (i32, i32*)* %2, i32 (i32, i32*)** %6, align 8
  %7 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 0
  %8 = load i32, i32* %7, align 4
  %9 = add i32 %8, 2
  %10 = call i32 @bar(i32 %9, %data_1* %3, i32 (i32, i32*)* @closure_1)
  ret i32 %10
}

define i32 @bar(i32 %0, i32* %1, i32 (i32, i32*)* %2) {
entry:
  %3 = alloca %data_2, align 8
  %4 = getelementptr inbounds %data_2, %data_2* %3, i32 0, i32 0
  store i32 %0, i32* %4, align 4
  %5 = getelementptr inbounds %data_2, %data_2* %3, i32 0, i32 1
  store i32* %1, i32** %5, align 8
  %6 = getelementptr inbounds %data_2, %data_2* %3, i32 0, i32 2
  store i32 (i32, i32*)* %2, i32 (i32, i32*)** %6, align 8
  %7 = getelementptr inbounds %data_2, %data_2* %3, i32 0, i32 2
  %8 = load i32 (i32, i32*)*, i32 (i32, i32*)** %7, align 8
  %9 = getelementptr inbounds %data_2, %data_2* %3, i32 0, i32 0
  %10 = load i32, i32* %9, align 4
  %11 = call i32 %8(i32 %10, %data_2* %3)
  ret i32 %11
}

define i32 @closure_1(i32 %0, i32* %1) {
entry:
  %2 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 0
  %3 = load i32, i32* %2, align 4
  %4 = add i32 %3, 3
  %5 = call i32 @bar(i32 %4, %data_1* %3, i32 (i32, i32*)* @closure_0)
  ret i32 %5
}

define i32 @closure_0(i32 %0, i32* %1) {
entry:
  %2 = getelementptr inbounds %data_1, %data_1* %3, i32 0, i32 2
  %3 = load i32 (i32, i32*)*, i32 (i32, i32*)** %2, align 8
  %4 = add i32 %0, %0
  %5 = call i32 %3(i32 %4, %data_1* %3)
  ret i32 %5
}

define i32 @closure_2(i32 %0, i32* %1) {
entry:
  ret i32 %0
}

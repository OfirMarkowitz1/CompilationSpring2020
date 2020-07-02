@.div_by_zero_error = constant [23 x i8] c"Error division by zero\00"
@.str_specifier = constant [4 x i8] c"%s\0A\00"
@.int_specifier = constant [4 x i8] c"%d\0A\00"
@.var4 = constant [12 x i8] c"val is true\00"
@.var5 = constant [13 x i8] c"val is false\00"
@.var8 = constant [5 x i8] c"true\00"
@.var9 = constant [5 x i8] c"true\00"

declare i32 @printf(i8*, ...)
declare void @exit(i32)

define void @print(i8*) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)
	ret void
}

define void @printi(i32) {
	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)
	ret void
}

define void @printByValue(i32 %0) {
	%stack_array_ptr = alloca [50 x i32]
	%var0 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	store i32 %0, i32* %var0
	%var1 = getelementptr [50 x i32], [50 x i32]* %stack_array_ptr, i32 0, i32 0
	%var2 = load i32, i32* %var1
	%var3 = trunc i32 %var2 to i1
	br i1 %var3, label %label_18, label %label_21
label_18:
	call void @print(i8* getelementptr ([12 x i8], [12 x i8]* @.var4, i32 0, i32 0))
	br label %label_24
label_21:
	call void @print(i8* getelementptr ([13 x i8], [13 x i8]* @.var5, i32 0, i32 0))
	br label %label_24
label_24:
	ret void
}

define void @main() {
	%stack_array_ptr = alloca [50 x i32]
	br label %label_30
label_30:
	br label %label_34
label_32:
	br label %label_34
label_34:
	%var6 = phi i32 [1, %label_30], [0, %label_32]
	call void @printByValue(i32 %var6)
	br label %label_40
label_38:
	br label %label_42
label_40:
	br label %label_42
label_42:
	%var7 = phi i32 [1, %label_38], [0, %label_40]
	call void @printByValue(i32 %var7)
	br label %label_46
label_46:
	call void @print(i8* getelementptr ([5 x i8], [5 x i8]* @.var8, i32 0, i32 0))
	br label %label_49
label_49:
	br label %label_52
	br label %label_52
label_52:
	br label %label_60
	br label %label_55
label_55:
	br label %label_57
label_57:
	call void @print(i8* getelementptr ([5 x i8], [5 x i8]* @.var9, i32 0, i32 0))
	br label %label_60
label_60:
	ret void
}

